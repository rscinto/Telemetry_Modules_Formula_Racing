#define HWSERIAL Serial1


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



void setup() {
  
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
    delay(50);
  }
    //Serial.println("Sending Test");
    //HWSERIAL.print("TEST");

  
}
