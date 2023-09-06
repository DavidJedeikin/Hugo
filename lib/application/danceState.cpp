#include "danceState.hpp"
#include "log.hpp"

DanceState::DanceState(Hardware& hardware) : hardware(hardware)
{
}

void DanceState::enter()
{
  LOG_INFO("Entering the %s", this->name());
}

void DanceState::runOnce()
{
  LOG_INFO("Running the %s once", this->name());
}

char const* DanceState::name()
{
  return "DanceState";
}
