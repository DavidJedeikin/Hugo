#pragma once
#include "linearMap.hpp"
#include <Adafruit_PWMServoDriver.h>

class Joints
{
 public:
  enum class Name
  {
    waist,
    right_shoulder,
    left_shoulder
  };

  struct Limits
  {
    int minAngle;
    int maxAngle;
  };

  Joints();

  void setAngle(Name name, int angle);

  [[nodiscard]] Limits getLimits(Name name) const;

 private:
  static constexpr uint32_t PULSE_SIGNAL_START{0};
  static constexpr uint32_t PWM_FREQUENCY_HZ{50};
  static constexpr uint32_t OSCILLATOR_FREQUENCY_HZ{25000000};

  static constexpr int WAIST_ANGLE_OFFSET{0};
  static constexpr int RIGHT_SHOULDER_ANGLE_OFFSET{0};
  static constexpr int LEFT_SHOULDER_ANGLE_OFFSET{0};

  static constexpr Limits WAIST_LIMITS{
      .minAngle = -45,
      .maxAngle = 45,
  };
  static constexpr Limits RIGHT_SHOULDER_LIMITS{
      .minAngle = 0,
      .maxAngle = 180,
  };
  static constexpr Limits LEFT_SHOULDER_LIMITS{
      .minAngle = 0,
      .maxAngle = 180,
  };

  static constexpr LinearMap::Params ANGLE_TO_DUTY_CYCLE_PARAMS{
      .inputMin = 0,
      .inputMax = 180,
      .outputMin = 55,
      .outputMax = 550,
  };

  LinearMap angleToDutyLinearCycleMap;
  Adafruit_PWMServoDriver pwmDriverBoard;

  [[nodiscard]] uint8_t servoNumber(Name name) const;
};
