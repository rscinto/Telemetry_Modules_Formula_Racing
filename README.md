# Telemetry Modules
Our goal is to connect the car to a laptop in a wireless fashion so that we can read live telemetry. 

CAN ==> TEENSY ==UART==> ESP32 ==WIFI==> ESP32 ==MAGIC==> Laptop (Sweet data graphs)

Plotting uses pySerial and Matplotlib

### See Issues for the current to do list
### Most up to date working code is in "Working Modules"



![alt text](Modules.jpg "Modules")


## How it works

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



```c

```


