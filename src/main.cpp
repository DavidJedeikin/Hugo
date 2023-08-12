#include "log.hpp"
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
  hugo::SerialLogger::getInstance().init();

  int rightEchoPin{11};
  int rightTriggerPin{7};

  int middleEchoPin{31};
  int middleTriggerPin{30};

  pinMode(rightTriggerPin, OUTPUT);
  pinMode(middleTriggerPin, OUTPUT);

  pinMode(rightEchoPin, INPUT);
  pinMode(middleEchoPin, INPUT);

  //////////////////////////////////////////////////////////////////////
  // Main Loop
  //////////////////////////////////////////////////////////////////////

  float distanceCmRight{0};
  float distanceCmMiddle{0};

  static constexpr float METERS_TO_CM{100.0F};
  static constexpr float SPEED_OF_SOUND{343.0};
  static constexpr float MICROSECONDS_TO_SECONDS{1 / 1000000.0F};
  while (true)
  {
    // Right
    digitalWrite(rightTriggerPin, LOW);
    delayMicroseconds(2); // Just to make sure you start high
    digitalWrite(rightTriggerPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(rightTriggerPin, LOW);

    distanceCmRight = (pulseIn(rightEchoPin, HIGH) / 2.0F) *
                      MICROSECONDS_TO_SECONDS * SPEED_OF_SOUND * METERS_TO_CM;

    delay(100);

    // Middle
    digitalWrite(middleTriggerPin, LOW);
    delayMicroseconds(2); // Just to make sure you start high
    digitalWrite(middleTriggerPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(middleTriggerPin, LOW);

    distanceCmMiddle = (pulseIn(middleEchoPin, HIGH) / 2.0F) *
                       MICROSECONDS_TO_SECONDS * SPEED_OF_SOUND * METERS_TO_CM;

    delay(100);

    LOG_RAW("Right: %.0f, Middle: %.0f", distanceCmRight, distanceCmMiddle);
  }
}
