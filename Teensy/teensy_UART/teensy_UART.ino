#include <HardwareSerial.h>
#define HWSERIAL Serial1

void setup() {
  
  Serial.begin(9600);
  HWSERIAL.begin(115200);
}
 
void loop() 
{
  
    Serial.print("Sending Test");
    HWSERIAL.print("TEST");

  //delay(5000);
}
