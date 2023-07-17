#include "serialLogger.hpp"
#include <Arduino.h>

// #include <Servo.h>
#include <Wire.h>
#include <string>

void setup()
{
  // Serial.begin(115200);
}

void loop()
{
  // int i = 0;
  // float height = 5.7F;
  // std::string name = "David";
  while (true)
  {
    hugo::SerialLogger::getInstance().log("TestPrint\r\n");
    // hugo::SerialLogger logger;
    // logger.log("TestLog");

    // height += 0.1;
    // Serial.printf(
    //     "A: %d, Height: %.2f, Name: %s \n\r", i++, height, name.c_str());
    delay(100);
  }
}