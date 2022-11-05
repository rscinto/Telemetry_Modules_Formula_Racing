#include "esp_now.h"
#include <WiFi.h>

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


unsigned long timer = 0;
long loopTime = 50000;   // microseconds

void setup() {
    Serial.begin(38400);
    timer = micros();

    // Set device as a Wi-Fi Station
    WiFi.mode(WIFI_STA);
    // Init ESP-NOW
    if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  // Once ESPNow is successfully Init, we will register for recv CB to
  // get recv packer info
  esp_now_register_recv_cb(OnDataRecv);

  WiFi.mode(WIFI_STA);
  Serial.print("The MAC address for this board is: ");
  Serial.println(WiFi.macAddress());
}

void loop() 
{
  timeSync(loopTime);

  float val = 10.0;
  //sendToPC(&val);
}







// callback function that will be executed when data is received
void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) 
{
  float number;
  memcpy(&number, incomingData, sizeof(number));
  sendToPC(&number);

  // Use this for testing in Serial Monitor if you're not seeing anything on the LCD display 
}







void timeSync(unsigned long deltaT)
{
  unsigned long currTime = micros();
  long timeToDelay = deltaT - (currTime - timer);
  if (timeToDelay > 5000)
  {
    delay(timeToDelay / 1000);
    delayMicroseconds(timeToDelay % 1000);
  }
  else if (timeToDelay > 0)
  {
    delayMicroseconds(timeToDelay);
  }
  else
  {
      // timeToDelay is negative so we start immediately
  }
  timer = currTime + timeToDelay;
}

void sendToPC(float* data)
{
  byte* byteData = (byte*)(data);
  Serial.write(byteData, 4);
}
