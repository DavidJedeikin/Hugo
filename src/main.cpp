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

  while (true)
  {
    joints.setAngle(Joints::Name::right_shoulder, 60);
    joints.setAngle(Joints::Name::left_shoulder, 60);
    delay(2000);
    joints.setAngle(Joints::Name::right_shoulder, 0);
    joints.setAngle(Joints::Name::left_shoulder, 0);
    delay(2000);
    joints.setAngle(Joints::Name::right_shoulder, -60);
    joints.setAngle(Joints::Name::left_shoulder, -60);
    delay(2000);
    joints.setAngle(Joints::Name::right_shoulder, 0);
    joints.setAngle(Joints::Name::left_shoulder, 0);
    delay(2000);
  }
}
