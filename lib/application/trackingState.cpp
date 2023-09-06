#include "trackingState.hpp"
#include "log.hpp"

TrackingState::TrackingState(Hardware& hardware) : hardware(hardware)
{
}

void TrackingState::enter()
{
  LOG_INFO("Entering the %s", this->name());
}

void TrackingState::runOnce()
{
  LOG_INFO("Running the %s once", this->name());
}

char const* TrackingState::name()
{
  return "TrackingState";
}
