#include "log.hpp"
#include "serialLogger.hpp"
#include <Arduino.h>

#include <Servo.h>
#include <Wire.h>
#include <string>

Servo rightArm;
Servo leftArm;
Servo base;
#define DELAY_MS 5

void setup()
{
  hugo::SerialLogger::getInstance().init();
  rightArm.attach(A0);
  leftArm.attach(A1);
  base.attach(A2);
}

void loop()
{
  int pos;
  LOG_RAW("%s", "Driving from: 0:180");
  for (pos = 0; pos <= 180; pos += 1)
  {
    rightArm.write(pos);
    leftArm.write(pos);
    base.write(pos);
    delay(DELAY_MS);
  }
  LOG_RAW("%s", "Driving from: 180:0");
  for (pos = 180; pos >= 0; pos -= 1)
  {
    rightArm.write(pos);
    leftArm.write(pos);
    base.write(pos);
    delay(DELAY_MS);
  }
}