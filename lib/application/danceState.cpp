#include "danceState.hpp"
#include "bodyMotion.hpp"
#include "log.hpp"

DanceState::DanceState(Hardware& hardware)
    : hardware(hardware), distanceToDanceSpeedMap(distanceToDanceSpeedParams),
      tooCloseState(*this), withinRangeState(*this), outOfRangeState(*this)
{
  this->outOfRangeState.enter();
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
  this->currentEyeColour = Eyes::Colour::light_blue;
  delay(100);
}

void DanceState::runOnce()
{
  auto desiredState = this->getDesiredState();
  LOG_INFO("Distance: %.2f, Min: %.2f, Max: %.2f",
           this->objectDistance,
           MIN_DISTANCE_CM,
           MAX_DISTANCE_CM);

  if (desiredState == nullptr)
  {
    LOG_WARN("%s", "Desired state is a nullptr");
    return;
  }
  if (this->currentState != desiredState)
  {
    desiredState->enter();
  }
  else
  {
    this->currentState->runOnce();
  }
}

char const* DanceState::name()
{
  return "DanceState";
}

//////////////////////////////////////////////////////////////////////
// Desired State Selector
//////////////////////////////////////////////////////////////////////

IState* DanceState::getDesiredState()
{
  this->objectDistance = this->hardware.sonarArray.getDistance().min;
  if (this->objectDistance < MIN_DISTANCE_CM)
  {
    return &this->tooCloseState;
  }
  if (this->objectDistance > MAX_DISTANCE_CM)
  {
    return &this->outOfRangeState;
  }
  return &this->withinRangeState;
}

//////////////////////////////////////////////////////////////////////
// TooCloseState
//////////////////////////////////////////////////////////////////////

DanceState::TooCloseState::TooCloseState(DanceState& parent) : parent(parent)
{
}

void DanceState::TooCloseState::enter()
{
  this->parent.currentState = this;
  LOG_INFO("Entering %s", this->name());
}

void DanceState::TooCloseState::runOnce()
{
  LOG_INFO("Running %s Once", this->name());
}

char const* DanceState::TooCloseState::name()
{
  return "TooCloseState";
}

//////////////////////////////////////////////////////////////////////
// WithinRangeState
//////////////////////////////////////////////////////////////////////

DanceState::WithinRangeState::WithinRangeState(DanceState& parent)
    : parent(parent)
{
}

void DanceState::WithinRangeState::enter()
{
  this->parent.currentState = this;
  LOG_INFO("Entering %s", this->name());
}

void DanceState::WithinRangeState::runOnce()
{
  LOG_INFO("Running %s Once", this->name());
}

char const* DanceState::WithinRangeState::name()
{
  return "WithinRangeState";
}

//////////////////////////////////////////////////////////////////////
// OutOfRangeState
//////////////////////////////////////////////////////////////////////

DanceState::OutOfRangeState::OutOfRangeState(DanceState& parent)
    : parent(parent)
{
}

void DanceState::OutOfRangeState::enter()
{
  this->parent.currentState = this;
  LOG_INFO("Entering %s", this->name());
}

void DanceState::OutOfRangeState::runOnce()
{
  LOG_INFO("Running %s Once", this->name());
}

char const* DanceState::OutOfRangeState::name()
{
  return "OutOfRangeState";
}
