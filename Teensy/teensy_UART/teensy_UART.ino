#define HWSERIAL Serial1

void setup() 
{ 
  Serial.begin(115200);
  HWSERIAL.begin(9600, SERIAL_8N1);
}
 
void loop() 
{
    
    for(float i = 50; i < 100; i=i+5)
    {
      byte* byteData = (byte*)(&i);
      HWSERIAL.write(byteData, 4);
      Serial.println(i);
      //HWSERIAL.print("TEST");
      delay(500);
    }



}
