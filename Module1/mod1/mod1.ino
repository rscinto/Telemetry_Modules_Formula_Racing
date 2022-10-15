#include <esp_now.h>
#include <WiFi.h>

//------------------------------------------------------START CLASS
class class_message
{
  public:
    int message_number;
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

long long int messages_sent = 0;

//------------------------------------------------------END CLASS

esp_now_peer_info_t peerInfo;

// REPLACE WITH YOUR RECEIVER MAC Address
uint8_t broadcastAddress[] = {0x24, 0x6F, 0x28, 0x7A, 0x93, 0x44};

// callback when data is sent
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("\r\nLast Packet Send Status:\t");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}

void setup() {

  Serial.begin(115200);


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
  class_message betterData;
  messages_sent++;
  betterData.message_number = messages_sent;

  betterData.data1 = 1000;
  betterData.data2 = 12000;
  betterData.data3 = 13000;
  betterData.data4 = 14000;
  betterData.data5 = 15000;
  betterData.data6 = 16000;
  betterData.data7 = 17000;
  betterData.data8 = 18000;
  betterData.data9 = 19000;
  betterData.data10 = 199000;

  esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *) &betterData, sizeof(betterData));

  if (result == ESP_OK) {
    Serial.println("Sent with success");
  }
  else {
    Serial.println("Error sending the data");
  }
  //delay(2000); // Send data every two seconds
}
