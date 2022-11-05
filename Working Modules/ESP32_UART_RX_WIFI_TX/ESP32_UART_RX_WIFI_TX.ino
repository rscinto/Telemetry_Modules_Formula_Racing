#include <esp_now.h>
#include <WiFi.h>
#define RXD2 16
#define TXD2 17

class class_message
{
  public:
    static int message_number;
    float data1;
    float data2;
    float data3;
    float data4;
    float data5;
    float data6;
    float data7;
    float data8;
    float data9;
    float data10;
    class_message(){ message_number++;}
  
};


//must be global
esp_now_peer_info_t peerInfo;

// REPLACE WITH YOUR RECEIVER MAC Address
uint8_t broadcastAddress[] = {0x24, 0x6F, 0x28, 0x79, 0xDE, 0x34};

// callback when data is sent
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("\r\nLast Packet Send Status:\t");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}

void setup() {

  Serial.begin(115200);
  Serial2.begin(9600, SERIAL_8N1, RXD2, TXD2);
  Serial.println("Serial Txd is on pin: "+String(TXD2));
  Serial.println("Serial Rxd is on pin: "+String(RXD2));


  // Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);

  // Init ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  // Once ESPNow is successfully Init, we will register for Send CB to
  // get the status of Trasnmitted packet
  esp_now_register_send_cb(OnDataSent);
  memcpy(peerInfo.peer_addr, broadcastAddress, 6);
  peerInfo.channel = 0;  
  peerInfo.encrypt = false;

  // Add peer        
  if (esp_now_add_peer(&peerInfo) != ESP_OK){
    Serial.println("Failed to add peer");
    return;
  }
}

void loop() {
  // Set values to send

 int numChars = 4;
 uint8_t receivedChars[numChars];
      
  while (Serial2.available()) 
  {
    Serial2.readBytes(receivedChars, numChars);
    //float i = float(Serial2.read());
    //byte* byteData = (byte*)(&i);

  esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *) receivedChars, sizeof(receivedChars));
  

  //Serial.println(i);
  //uint8_t bytes[4] = receivedChars; // fill this array with the four bytes you received
  //static_assert(sizeof(float) == 4, "float size is expected to be 4 bytes");
  float f;
  memcpy (&f, receivedChars, 4);
  Serial.println(f);
  

    if (result == ESP_OK) {
      Serial.println("Sent with success");
    }
    else {
      Serial.println("Error sending the data");
    }
  }

  




  //delay(2000); // Send data every two seconds
}
