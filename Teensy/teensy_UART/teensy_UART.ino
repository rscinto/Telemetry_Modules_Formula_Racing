#define HWSERIAL Serial1

void setup() {
  
  Serial.begin(115200);
  HWSERIAL.begin(9600, SERIAL_8N1);
}
 
void loop() 
{
  
    Serial.println("Sending Test");
    HWSERIAL.print("TEST");

  delay(500);
}
