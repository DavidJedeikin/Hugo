#include "log.hpp"
#include "serialLogger.hpp"
#include <Arduino.h>

// #include <Servo.h>
#include <Wire.h>
#include <string>

void setup()
{
  hugo::SerialLogger::getInstance().init();
}

void loop()
{
  int i = 0;
  float height = 5.7F;
  std::string name = "David";
  while (true)
  {
    LOG_RAW("A: %d, Height: %.2f, Name: %s \n\r", i++, height, name.c_str());
    delay(100);
  }
}