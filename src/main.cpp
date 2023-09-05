#include "eyes.hpp"
#include "joints.hpp"
#include "log.hpp"
#include "sonarArray.hpp"
#include <Adafruit_TinyUSB.h> // for Serial
#include <Arduino.h>
#include <Wire.h>

void setup()
{
}

void loop()
{
  //////////////////////////////////////////////////////////////////////
  // Initialisation
  //////////////////////////////////////////////////////////////////////
  SerialLogger::getInstance().init();

  // Mode input
  int modeSelectGpioIn{A3};
  pinMode(modeSelectGpioIn, INPUT_PULLDOWN);

  Eyes eyes;
  eyes.setColour(Eyes::Colour::light_blue);
  SonarArray sonarArray;

  Joints joints;
  Joints::Limits limits = joints.getLimits(Joints::Name::left_shoulder);
  while (true)
  {
    for (int i = limits.minAngle; i < limits.maxAngle; i++)
    {
      joints.setAngle(Joints::Name::left_shoulder, i);
      delay(5);
    }

    for (int i = limits.maxAngle; i > limits.minAngle; i--)
    {
      joints.setAngle(Joints::Name::left_shoulder, i);
      delay(5);
    }
  }
}
