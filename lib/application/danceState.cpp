#include "danceState.hpp"
#include "bodyMotion.hpp"
#include "log.hpp"

DanceState::DanceState(Hardware& hardware)
    : hardware(hardware), distanceToDanceSpeedMap(distanceToDanceSpeedParams)
{
}

void DanceState::enter()
{
  LOG_INFO("Entering the %s", this->name());
  for (int i = 0; i < 5; i++)
  {
    this->hardware.eyes.setColour(Eyes::Colour::red);
    delay(100);
    this->hardware.eyes.setColour(Eyes::Colour::light_blue);
    delay(100);
  }
  delay(100);
}

void DanceState::runOnce()
{
  float distance = this->hardware.sonarArray.getDistance().min;

  int sweepSpeedMs =
      static_cast<int>(this->distanceToDanceSpeedMap.getOutput(distance));

  LOG_INFO("Distance: %.0f, Speed: %d", distance, sweepSpeedMs);

  if (distance < 130)
  {
    this->hardware.eyes.setColour(Eyes::Colour::red);
    BodyMotion::minFullSweep(this->hardware.joints, sweepSpeedMs, -15);
    delay(250);
  }
  else
  {
    this->hardware.eyes.setColour(Eyes::Colour::light_blue);
  }
}

char const* DanceState::name()
{
  return "DanceState";
}
