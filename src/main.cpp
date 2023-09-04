#include "eyes.hpp"
#include "log.hpp"
#include "pidController.hpp"
#include "sonarArray.hpp"
#include <Adafruit_TinyUSB.h> // for Serial
#include <Arduino.h>
#include <Servo.h>
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

  // // Servo control
  // static constexpr float SERVO_MIN{90};
  // static constexpr float SERVO_MAX{200};

  // float servoAngle{135};
  // Servo baseServo;
  // baseServo.attach(A4);

  // PidController::Parameters params{.Kp = 0.4F,
  //                                  .Kd = 0.0F,
  //                                  .Ki = 0.0F,
  //                                  .timestepMs = 60,
  //                                  .maxControlSignal = SERVO_MAX,
  //                                  .minControlSignal = -SERVO_MAX};

  // PidController pidController(params);

  // Mode input
  int modeSelectGpioIn{A3};
  pinMode(modeSelectGpioIn, INPUT_PULLDOWN);

  Eyes eyes;
  eyes.setColour(Eyes::Colour::light_blue);
  // SonarArray sonarArray;

  while (true)
  {
    // eyes.setColour(Eyes::Colour::red);
    // LOG_RAW("Distance: %s", sonarArray.getDistance().toString());

    // float controlSignal =
    //     pidController.getControlSignal(differenceFiltered, 0.0);

    // servoAngle = std::clamp(servoAngle + controlSignal, SERVO_MIN,
    // SERVO_MAX); baseServo.write(servoAngle);
  }
}
