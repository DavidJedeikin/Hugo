#include "joints.hpp"

Joints::Joints() : angleToDutyLinearCycleMap(ANGLE_TO_DUTY_CYCLE_PARAMS)
{
  this->pwmDriverBoard.begin();
  this->pwmDriverBoard.setPWMFreq(PWM_FREQUENCY_HZ);
  this->pwmDriverBoard.setOscillatorFrequency(OSCILLATOR_FREQUENCY_HZ);
  delay(10);
}

void Joints::setAngle(Name name, int angle)
{
  uint32_t servoNumber = this->servoNumber(name);
  uint32_t dutyCycle = this->angleToDutyLinearCycleMap.getOutput(angle);
  this->pwmDriverBoard.setPWM(servoNumber, PULSE_SIGNAL_START, dutyCycle);
}

Joints::Limits Joints::getLimits(Name name) const
{
  switch (name)
  {
    case Name::waist:
      return WAIST_LIMITS;
    case Name::right_shoulder:
      return RIGHT_SHOULDER_LIMITS;
    case Name::left_shoulder:
      return LEFT_SHOULDER_LIMITS;
  }
}

uint8_t Joints::servoNumber(Name name) const
{
  switch (name)
  {
    case Name::waist:
      return 1;
    case Name::right_shoulder:
      return 2;
    case Name::left_shoulder:
      return 0;
  }
}
