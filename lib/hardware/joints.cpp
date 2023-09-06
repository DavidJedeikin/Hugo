#include "joints.hpp"
#include "log.hpp"

Joints::Joints() : angleToDutyLinearCycleMap(ANGLE_TO_DUTY_CYCLE_PARAMS)
{
  this->pwmDriverBoard.begin();
  this->pwmDriverBoard.setPWMFreq(PWM_FREQUENCY_HZ);
  this->pwmDriverBoard.setOscillatorFrequency(OSCILLATOR_FREQUENCY_HZ);
  delay(10);

  this->setAngle(Name::left_shoulder, 0);
  this->setAngle(Name::right_shoulder, 0);
  LOG_INFO("Setting %s & %s to position: 0 and delaying for 3 seconds\r\n",
           this->toString(Name::left_shoulder),
           this->toString(Name::right_shoulder));
  delay(3000);
}

void Joints::setAngle(Name name, int angle)
{
  int offsetAngle = this->accountForZeroOffset(name, angle);
  uint32_t servoNumber = this->servoNumber(name);
  uint32_t dutyCycle = static_cast<uint32_t>(
      this->angleToDutyLinearCycleMap.getOutput(offsetAngle));
  this->pwmDriverBoard.setPWM(servoNumber, PULSE_SIGNAL_START, dutyCycle);

  LOG_INFO(
      "%s, Angle: %d, Offset Angle: %d, ServoNumber: %u, DutyCycle: %u\r\n",
      this->toString(name),
      angle,
      offsetAngle,
      servoNumber,
      dutyCycle);
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
      return 2;
    case Name::right_shoulder:
      return 1;
    case Name::left_shoulder:
      return 0;
  }
}

int Joints::accountForZeroOffset(Name name, int angle) const
{
  switch (name)
  {
    case Name::waist:
      return WAIST_ZERO_OFFSET + angle;
    case Name::right_shoulder:
      return RIGHT_SHOULDER_ZERO_OFFSET - angle;
    case Name::left_shoulder:
      return angle + LEFT_SHOULDER_ZERO_OFFSET;
  }
}

std::string Joints::toString(Name name)
{
  switch (name)
  {
    case Name::waist:
      return "WAIST";
    case Name::right_shoulder:
      return "RIGHT_SHOULDER";
    case Name::left_shoulder:
      return "LEFT_SHOULDER";
  }
}
