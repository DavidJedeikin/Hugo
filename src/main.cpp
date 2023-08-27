#include "log.hpp"
#include "pidController.hpp"
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
  static constexpr float SERVO_MIN{90};
  static constexpr float SERVO_MAX{200};

  float servoAngle{135};
  Servo baseServo;
  baseServo.attach(A4);

  PidController::Parameters params{.Kp = 0.4F,
                                   .Kd = 0.0F,
                                   .Ki = 0.0F,
                                   .timestepMs = 60,
                                   .maxControlSignal = SERVO_MAX,
                                   .minControlSignal = -SERVO_MAX};

  PidController pidController(params);

  // Sonar
  int rightEchoPin{A1};
  int rightTriggerPin{A0};

  int leftEchoPin{11};
  int leftTriggerPin{31};

  float rightDistance{0};
  float leftDistance{0};

  LinearFirstOrderFiler leftFilter(0.5);
  LinearFirstOrderFiler rightFilter(0.5);

  pinMode(rightTriggerPin, OUTPUT);
  pinMode(leftTriggerPin, OUTPUT);
  pinMode(rightEchoPin, INPUT);
  pinMode(leftEchoPin, INPUT);

  // Eyes
  int redEyes{A0};
  int greenEyes{A1};
  int blueEyes{A2};
  pinMode(redEyes, OUTPUT);
  pinMode(greenEyes, OUTPUT);
  pinMode(blueEyes, OUTPUT);

  //////////////////////////////////////////////////////////////////////
  // Main Loop
  //////////////////////////////////////////////////////////////////////

  while (true)
  {

    // rightDistance = getDistance(rightTriggerPin, rightEchoPin);
    // float rightFiltered = rightFilter.getVal(rightDistance);
    // delay(30);

    // leftDistance = getDistance(leftTriggerPin, leftEchoPin);
    // float leftFiltered = leftFilter.getVal(leftDistance);

    // delay(30);

    // float differenceFiltered = rightFiltered - leftFiltered;

    // if (differenceFiltered < 15 && differenceFiltered > -15)
    // {
    //   differenceFiltered = 0;
    // }

    // float controlSignal =
    //     pidController.getControlSignal(differenceFiltered, 0.0);

    // servoAngle = std::clamp(servoAngle + controlSignal, SERVO_MIN,
    // SERVO_MAX); baseServo.write(servoAngle);

    // LOG_RAW("RawRight: %.2f", rightDistance);
    // LOG_RAW("FilteredRight: %.2f", rightFiltered);

    // LOG_RAW(">DifferenceFiltered:%.2f", differenceFiltered);
    // LOG_RAW(">ControlSignal:%.2f", controlSignal);
    // LOG_RAW("Difference: %.2f, ControlSignal: %.2f, ServoAngle: %.2f",
    //         differenceFiltered,
    //         controlSignal,
    //         servoAngle);

    LOG_RAW("%s", "OFF");
    digitalWrite(redEyes, HIGH);
    digitalWrite(blueEyes, HIGH);
    digitalWrite(greenEyes, HIGH);
    delay(3000);

    LOG_RAW("%s", "Red eyes");
    digitalWrite(redEyes, LOW);
    digitalWrite(blueEyes, HIGH);
    digitalWrite(greenEyes, HIGH);
    delay(3000);

    LOG_RAW("%s", "Blue eyes");
    digitalWrite(redEyes, HIGH);
    digitalWrite(blueEyes, LOW);
    digitalWrite(greenEyes, HIGH);
    delay(3000);

    LOG_RAW("%s", "Green eyes");
    digitalWrite(redEyes, HIGH);
    digitalWrite(blueEyes, HIGH);
    digitalWrite(greenEyes, LOW);
    delay(3000);
  }
}
