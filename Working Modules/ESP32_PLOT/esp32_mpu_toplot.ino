#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>

Adafruit_MPU6050 mpu;
unsigned long timer = 0;
long loopTime = 50000;   // microseconds

void setup() {
  Serial.begin(38400);
  mpu.begin();
  mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
  timer = micros();
}

void loop() {
  timeSync(loopTime);
  sensors_event_t a, g, t;
  mpu.getEvent(&a, &g, &t);
  float val = a.acceleration.x;
  sendToPC(&val);
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
