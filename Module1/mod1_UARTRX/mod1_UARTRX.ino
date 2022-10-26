#include <HardwareSerial.h>

//#define RXD2 40
//#define TXD2 41


   
 void setup() 
 {
  // Initialize "debug" serial port
  // The data rate must be much higher than the "link" serial port
  Serial.begin(9600);

  // Initialize the "link" serial port
  // Use a low data rate to reduce the error ratio
  Serial1.begin(115200);
 }

 
 void loop() 
 {
  
  if(Serial1.available())
  {
     Serial.write(Serial1.read());  
  }

 }
