#include "log.hpp"
#include <Adafruit_TinyUSB.h> // for Serial
#include <Arduino.h>
#include <Servo.h>
#include <Wire.h>

class LinearMap
{
 public:
  LinearMap(float xMin, float xMax, float yMin, float yMax)
  {
    this->m = (yMax - yMin) / (xMax - xMin);
  }

  float getY(float x)
  {
    return this->m * x + this->c;
  }

 private:
  float m;
  float c;
};

void setup()
{
}

void loop()
{
  //////////////////////////////////////////////////////////////////////
  // Initialisation
  //////////////////////////////////////////////////////////////////////
  hugo::SerialLogger::getInstance().init();

  // Servo
  int potPin{A5};
  LinearMap potToServoAngleMap(0, 942, 0, 180);
  Servo baseServo;
  baseServo.attach(A4);

  // Sonar
  int rightEchoPin{15};
  int rightTriggerPin{7};
  int leftEchoPin{11};
  int leftTriggerPin{31};
  pinMode(rightTriggerPin, OUTPUT);
  pinMode(leftTriggerPin, OUTPUT);
  pinMode(rightEchoPin, INPUT);
  pinMode(leftEchoPin, INPUT);

  //////////////////////////////////////////////////////////////////////
  // Main Loop
  //////////////////////////////////////////////////////////////////////

  uint32_t potValue{0}; // 0 -> 942

  float difference{0};
  float distanceCmRight{0};
  float distanceCmLeft{0};

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

    delay(50);

    // Left
    digitalWrite(leftTriggerPin, LOW);
    delayMicroseconds(2); // Just to make sure you start high
    digitalWrite(leftTriggerPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(leftTriggerPin, LOW);

    distanceCmLeft = (pulseIn(leftEchoPin, HIGH) / 2.0F) *
                     MICROSECONDS_TO_SECONDS * SPEED_OF_SOUND * METERS_TO_CM;

    delay(50);

    difference = distanceCmLeft - distanceCmRight;
    LOG_RAW("Right: %.0f, Left: %.0f, Difference: %.2f",
            distanceCmRight,
            distanceCmLeft,
            difference);

    // Servo

    potValue = analogRead(potPin);
    float servoWriteVal = potToServoAngleMap.getY(potValue);
    baseServo.write(servoWriteVal);
    LOG_RAW("POT Val: %u, Servo Angle: %.2f", potValue, servoWriteVal);
  }
}
