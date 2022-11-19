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
