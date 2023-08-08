#include "log.hpp"
#include "serialLogger.hpp"
#include <Arduino.h>

#include <Servo.h>
#include <Wire.h>
#include <string>

Servo rightArm;
Servo leftArm;
Servo base;

// #define START 80
// #define END 175

#define START 0
#define END 180

#define DELAY_MS 200

void setup()
{
  hugo::SerialLogger::getInstance().init();
  rightArm.attach(A0);
  leftArm.attach(A1);
  // base.attach(A2);
}

void loop()
{
  int pos;
  LOG_RAW("Driving from: %u:%u, Delay: %ums", START, END, DELAY_MS);
  for (pos = START; pos <= END; pos += 1)
  {
    rightArm.write(pos);
    leftArm.write(pos);
    base.write(pos);
    delay(DELAY_MS);
  }
  LOG_RAW("Driving from: %u:%u, Delay: %ums", END, START, DELAY_MS);
  for (pos = END; pos >= START; pos -= 1)
  {
    rightArm.write(pos);
    leftArm.write(pos);
    base.write(pos);
    delay(DELAY_MS);
  }
}