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
  this->hardware.eyes.crossFade(Eyes::Colour::red, Eyes::Colour::blue, 1000);
  delay(500);
  this->hardware.eyes.crossFade(Eyes::Colour::blue, Eyes::Colour::red, 1000);
  delay(500);
}

char const* TrackingState::name()
{
  return "TrackingState";
}
