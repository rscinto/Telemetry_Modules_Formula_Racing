# Telemetry Modules
Our goal is to connect the car to a laptop in a wireless fashion so that we can read live telemetry. 

CAN ==> TEENSY ==UART==> ESP32 ==WIFI==> ESP32 ==MAGIC==> Laptop (Sweet data graphs)

Plotting uses pySerial and Matplotlib

### See Issues for the current to do list
### Most up to date working code is in "Working Modules"



![alt text](Modules.jpg "Modules")


# How it works

### Teensy Module
```c
#define HWSERIAL Serial1

void setup() 
{
  Serial.begin(74880);
  HWSERIAL.begin(9600, SERIAL_8N1);
}
 
void loop() 
{
  for(float modulator= 50; modulator < 150; modulator++)
  {
    float* sender = &modulator;
    byte* byteData = (byte*)(sender);
    HWSERIAL.write(byteData, 4);
    Serial.println(modulator);
    delay(50);
  } 
}
```

### ESP32 UART Reciever, WIFI Sender

```c
#include <esp_now.h>
#include <WiFi.h>
#define RXD2 16
#define TXD2 17

//must be global
esp_now_peer_info_t peerInfo;

// REPLACE WITH YOUR RECEIVER MAC Address
uint8_t broadcastAddress[] = {0x24, 0x0A, 0xC4, 0x61, 0x4F, 0x44};
//24:0A:C4:61:4F:44

// callback when data is sent
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("\r\nLast Packet Send Status:\t");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}

void setup() {

  Serial.begin(115200); //Serial for debugging
  
  Serial2.begin(9600, SERIAL_8N1, RXD2, TXD2); //Serial for over wire transfer



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

void loop() 
{
  // Set values to send
 int numChars = 4;
 uint8_t receivedChars[numChars];

//Receiving data from the serial
  while (Serial2.available()) 
  {
    Serial2.readBytes(receivedChars, numChars);
    esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *) receivedChars, sizeof(receivedChars));
    
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

}
```

### ESP32 UART Reciever, WIFI Sender

```c
#include "esp_now.h"
#include <WiFi.h>

unsigned long timer = 0;
long loopTime = 50000;   // microseconds

void setup() {
    Serial.begin(38400);
    timer = micros();

    // Set device as a Wi-Fi Station
    WiFi.mode(WIFI_STA);
    // Init ESP-NOW
    if (esp_now_init() != ESP_OK) {
    //Serial.println("Error initializing ESP-NOW");
    return;
  }

  // Once ESPNow is successfully Init, we will register for recv CB to
  // get recv packer info
  esp_now_register_recv_cb(OnDataRecv);
}

void loop() 
{
  timeSync(loopTime);

  //Print to check if MAC Address changes
  //WiFi.mode(WIFI_STA);
  //Serial.print("The MAC address for this board is: ");
  //Serial.println(WiFi.macAddress());
}







// callback function that will be executed when data is received
void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) 
{
  //Serial.println("I got a message");
  float number;
  memcpy(&number, incomingData, sizeof(number));
  sendToPC(&number);
  //Serial.println(number);

  // Use this for testing in Serial Monitor if you're not seeing anything on the LCD display 
}


void sendToPC(float* data)
{
  byte* byteData = (byte*)(data);
  Serial.write(byteData, 4);
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
```

