#include "log.hpp"
#include <Adafruit_TinyUSB.h> // for Serial
#include <Arduino.h>
#include <Servo.h>
#include <Wire.h>

static constexpr float METERS_TO_CM{100.0F};
static constexpr float SPEED_OF_SOUND{343.0};
static constexpr float MICROSECONDS_TO_SECONDS{1 / 1000000.0F};

class LinearMap
{
 public:
  LinearMap(float xMin, float xMax, float yMin, float yMax)
  {
    this->m = (yMax - yMin) / (xMax - xMin);
    this->c = yMax - this->m * xMax;
  }

  float getY(float x)
  {
    return this->m * x + this->c;
  }

 private:
  float m;
  float c;
};

class LinearFirstOrderFiler
{
 public:
  LinearFirstOrderFiler(float alpha) : alpha(alpha)
  {
  }

  float getVal(float measurement)
  {
    float nextVal =
        (this->alpha * measurement) + ((1 - this->alpha) * this->val);
    this->val = nextVal;
    return this->val;
  }

 private:
  float val{0};
  float alpha{0};
};

float getDistance(uint32_t triggerPin, uint32_t echoPin)
{
  digitalWrite(triggerPin, LOW);
  delayMicroseconds(2);
  digitalWrite(triggerPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(triggerPin, LOW);
  return (pulseIn(echoPin, HIGH) / 2.0F) * MICROSECONDS_TO_SECONDS *
         SPEED_OF_SOUND * METERS_TO_CM;
}

void setup()
{
}

void loop()
{
  //////////////////////////////////////////////////////////////////////
  // Initialisation
  //////////////////////////////////////////////////////////////////////
  hugo::SerialLogger::getInstance().init();

  // Servo control
  int potPin{A5};

  float servoAngle{135};
  float controlSignal{0};
  float proportionalGain{0};

  Servo baseServo;

  baseServo.attach(A4);

  // Sonar
  int rightEchoPin{15};
  int rightTriggerPin{7};
  int leftEchoPin{11};
  int leftTriggerPin{31};

  float difference{0};
  float rightDistance{0};
  float leftDistance{0};
  LinearFirstOrderFiler leftFilter(0.1);
  LinearFirstOrderFiler rightFilter(0.1);

  pinMode(rightTriggerPin, OUTPUT);
  pinMode(leftTriggerPin, OUTPUT);
  pinMode(rightEchoPin, INPUT);
  pinMode(leftEchoPin, INPUT);

  //////////////////////////////////////////////////////////////////////
  // Main Loop
  //////////////////////////////////////////////////////////////////////

  while (true)
  {

    rightDistance = getDistance(rightTriggerPin, rightEchoPin);
    float rightFiltered = rightFilter.getVal(rightDistance);
    delay(50);

    leftDistance = getDistance(leftTriggerPin, leftEchoPin);
    float leftFiltered = leftFilter.getVal(leftDistance);

    delay(50);

    difference = leftDistance - rightDistance;
    float differenceFiltered = leftFiltered - rightFiltered;

    LOG_RAW(">LeftRaw:%.2f", leftDistance);
    LOG_RAW(">RightRaw:%.2f\r\n", rightDistance);
    LOG_RAW(">DifferenceRaw:%.2f\r\n", difference);

    LOG_RAW(">LeftFiltered:%.2f", leftFiltered);
    LOG_RAW(">RightFiltered:%.2f\r\n", rightFiltered);
    LOG_RAW(">DifferenceFiltered:%.2f\r\n", differenceFiltered);

    // Control Loop
    // proportionalGain = static_cast<float>(analogRead(potPin)) * 0.01;
    // controlSignal = proportionalGain * difference;
    // servoAngle = std::clamp(servoAngle + controlSignal, 90.0F, 180.0F);
    // baseServo.write(servoAngle);
  }
}
