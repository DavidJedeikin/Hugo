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

  uint32_t wait{5};

  for (int i = 0; i < 20; i++)
  {
    joints.setAngle(Joints::Name::left_shoulder, i);
    joints.setAngle(Joints::Name::waist, i);
    delay(wait);
  }
  while (true)
  {
    for (int i = 20; i > -20; i--)
    {
      joints.setAngle(Joints::Name::left_shoulder, i);
      joints.setAngle(Joints::Name::right_shoulder, -1 * i);
      joints.setAngle(Joints::Name::waist, i);
      delay(wait);
    }
    for (int i = -20; i < 20; i++)
    {
      joints.setAngle(Joints::Name::left_shoulder, i);
      joints.setAngle(Joints::Name::right_shoulder, -1 * i);
      joints.setAngle(Joints::Name::waist, i);
      delay(wait);
    }
  }
}
