#include "trackingState.hpp"
#include "log.hpp"

TrackingState::TrackingState(Hardware& hardware) : hardware(hardware)
{
}

void TrackingState::enter()
{
  LOG_INFO("Entering the %s", this->name());
  this->hardware.eyes.setColour(Eyes::Colour::green);
}

void TrackingState::runOnce()
{
  this->hardware.eyes.crossFade(
      Eyes::Colour::light_blue, Eyes::Colour::red, 2000);
  delay(1000);
  this->hardware.eyes.crossFade(
      Eyes::Colour::red, Eyes::Colour::light_blue, 2000);
}

char const* TrackingState::name()
{
  return "TrackingState";
}
