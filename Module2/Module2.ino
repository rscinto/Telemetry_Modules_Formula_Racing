/* WiFi Peer-to-Peer example, Receiver Sketch
 * Rob Reynolds, SparkFun Electronics, November 2021
 * This example uses a pair of SparkFun ESP32 Thing Plus Wroom modules
 * (https://www.sparkfun.com/products/15663, a SparkFun Qwiic Environmental
 * Combo Breakout (https://www.sparkfun.com/products/14348), and a SparkFun
 * Qwiic 20x4 SerLCD - RGB Backlight (https://www.sparkfun.com/products/16398).
 * 
 * Feel like supporting our work? Buy a board from SparkFun!
 * https://www.sparkfun.com/
 * 
 * License: MIT. See license file for more information but you can
 * basically do whatever you want with this code.
 * 
 * Based on original code by 
 * Rui Santos
 * Complete project details at https://RandomNerdTutorials.com/esp-now-esp32-arduino-ide/
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files.
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
*/

#include "esp_now.h"
#include <WiFi.h>

//------------------------------------------------------START CLASS
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
int class_message::message_number = 0;

//------------------------------------------------------END CLASS





// Create a struct_message called myData
class_message betterData;


void setup() {
  // Initialize Serial Monitor
  Serial.begin(115200);

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
}

void loop() {

  //WiFi.mode(WIFI_STA);
  //Serial.print("The MAC address for this board is: ");
  //Serial.println(WiFi.macAddress());

}



/*
// callback function that will be executed when data is received
void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) {
  memcpy(&myData, incomingData, sizeof(myData));

  // Use this for testing in Serial Monitor if you're not seeing anything on the LCD display 
  Serial.print("Temperature F: ");
  Serial.println(myData.a);
  Serial.print("Humidity: ");
  Serial.println(myData.b);
  Serial.print("Pressure: ");
  Serial.println(myData.c);
  Serial.println();

}
*/

// callback function that will be executed when data is received
void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) 
{
  memcpy(&betterData, incomingData, sizeof(betterData));

  // Use this for testing in Serial Monitor if you're not seeing anything on the LCD display 
  String message = String(betterdata);

  Serial.println(betterData);
  Serial.println();

}
