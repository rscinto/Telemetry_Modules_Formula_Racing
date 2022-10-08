/*What was added into transciever firmware 3.4
- Updated linked list to accomodate for a bug caused by the dual cores of the esp32.
The bug was caused because one core would delete and the other core would add.
The solution to this bug was to leave all the adding and deleteing to CoreA and CoreB
sends the received data over serial.
The linked list was updated so that the node now contains the "ready_to_delete" boolean.
It deletes nodes from the head and adds from the tail.
There is also a third pointer, "send_ptr", now that points at the next data to send over serial.
After the send_ptr sends the data on the node it is pointing at, it sets the ready_to_delete to true,
which then is deleted in CoreA.
The deletion occurs at CoreA in the OnDataRecv function.

However, this presents a problem because OnDataRecv is only called when a message is received.
So, once it has stopped reveiving for a while, it starts deleting from CoreB.
It does so by using the checking if the time since the last send from CoreA is over the "TIME_THRESHOLD".

Possible Concerns:
- TIME_THRESHOLD is not optimized 
*/

// Important Headers
#include <WiFi.h>
#include "esp_wifi_internal.h"
#include <esp_now.h>

#define ESPNOW_WIFI_IF ESP_IF_WIFI_STA 
#define TIME_THRESHOLD 1000//milliseconds

//Important Variables
uint8_t broadcastAddress[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};

//STRUCTURES AND ENUMERATIONS
enum STATES {Disconnected, Connected};
STATES state = Disconnected;

typedef struct message {
  uint8_t type;
  uint8_t dlc;
  uint8_t xcvrID;
  uint16_t msgNum;
  uint8_t payload[63];
  uint16_t checkSum;
} message;

typedef struct Node{
  uint8_t* frame_ptr;
  int frame_len;
  boolean ready_to_delete;
  Node* next;
} Node;

Node* head = nullptr;
Node* tail = nullptr;
Node* send_ptr = nullptr;

int list_size = 0;
int time_callback = 0;//might want to use a different type

uint16_t IDs[16];

//FUNCTIONS
uint8_t tryToConnect();
uint8_t tryReadMessage(message* m);
uint8_t sendMessage(message* m);
uint8_t compareArray(uint8_t* a, uint8_t b, uint8_t n);
uint8_t frameToMsg(const uint8_t* data_in, message* m);
uint8_t compareMessage(message* m1, message* m2);
uint8_t msgToFrame( message* m, uint8_t* a);
void printMessage(message* m1);
void SendSerialData();
bool BroadcastData(message* m); 
void test();

//VARIABLES
uint8_t buf[2] = {0,0};

//CONSTANTS
uint8_t startFlag[2] = {0xFF, 0xFF};

//GLOBAL STRUCTURES
message msg;
message handshake;
message acknowledge;


//---------------------------------CODE-------------------------------------//

void setup() {
  // Init Serial Monitor
  Serial.begin(115200);
 
  WiFi.disconnect();

  // Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);
  
  /*Stop wifi to change config parameters*/
  esp_wifi_stop();
  esp_wifi_deinit();
  
  /*Disabling AMPDU is necessary to set a fix rate*/
  wifi_init_config_t my_config = WIFI_INIT_CONFIG_DEFAULT(); //We use the default config ...
  my_config.ampdu_tx_enable = 0;                             //... and modify only what we want.
  esp_wifi_init(&my_config);                                 //set the new config
  
  esp_wifi_start();
  
  /*You'll see that in the next subsection ;)*/
  esp_wifi_set_channel(1, WIFI_SECOND_CHAN_NONE);
  
  /*set the rate*/
  esp_wifi_internal_set_fix_rate(ESPNOW_WIFI_IF, true, WIFI_PHY_RATE_48M);
  
  // Init ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  
  // Register peer
  esp_now_peer_info_t peerInfo;
  memcpy(peerInfo.peer_addr, broadcastAddress, 6);
  peerInfo.channel = 0;  
  peerInfo.encrypt = false;
  
  // Add peer        
  if (esp_now_add_peer(&peerInfo) != ESP_OK){
    Serial.println("Failed to add peer");
    return;
  }
  
  // Register for a callback function that will be called when data is received
  esp_now_register_recv_cb(OnDataRecv);


  //REST OF SETUP

  //Initializes the two main messages in the handshake
  handshake.type = 0;
  handshake.dlc = 0;
  handshake.xcvrID = 0;
  handshake.msgNum = 0;
  handshake.checkSum = 0;
  
  acknowledge.type = 0;
  acknowledge.dlc = 0;
  acknowledge.xcvrID = 0;
  acknowledge.msgNum = 0;
  acknowledge.checkSum = 0;

  delay(200);

}


void loop() {
  
  if(state == Disconnected)
  {
    // Tries to connect
    if(tryToConnect()) 
    {
      //sendMessage(&acknowledge);
      printMessage(&acknowledge);
      state = Connected;
      Serial.println("Connected");
    }
  }
  else if(state == Connected)
  {
    if(Serial.available()){
      buf[1] = buf[0];
      buf[0] = Serial.read();
      if(compareArray(buf, startFlag, 2)) {
        if(tryReadMessage(&msg)) {
          Serial.println("Message succesfully stored");
          if(BroadcastData(&msg) != ESP_OK) {
            Serial.println("Failed to send data");
          }
          Serial.println();
        }
        buf[0] = 0;//This resets the buffer so that it does not get read in if it reads in only one 0xFF
        buf[1] = 0;
      }
    }
    if(list_size) {//if linked list is not empty ***add try catch
      SendSerialData();
    }
  }
}
