#include "trackingState.hpp"
#include "bodyMotion.hpp"
#include "log.hpp"

TrackingState::TrackingState(Hardware& hardware)
    : hardware(hardware), tooCloseState(*this), withinRangeState(*this),
      outOfRangeState(*this)
{
  this->outOfRangeState.enter();
}

void TrackingState::enter()
{
  LOG_INFO("Entering the %s", this->name());
  this->hardware.eyes.setColour(Eyes::Colour::green);
  BodyMotion::setArms(this->hardware.joints, ARM_ANGLE);
}

void TrackingState::runOnce()
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

char const* TrackingState::name()
{
  return "TrackingState";
}

//////////////////////////////////////////////////////////////////////
// States
//////////////////////////////////////////////////////////////////////

IState* TrackingState::getDesiredState()
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
// Base State
//////////////////////////////////////////////////////////////////////

TrackingState::State::State(TrackingState& parent,
                            std::string&& stateName,
                            Eyes::Colour eyeColour,
                            int eyeTransitionTime)
    : parent(parent), stateName(stateName), eyeColour(eyeColour),
      eyeTransitionTime(eyeTransitionTime)
{
}

void TrackingState::State::enter()
{
  LOG_INFO("Entering %s::%s", this->parent.name(), this->name());
  this->parent.waistAngle = 0;
  this->parent.hardware.joints.setAngle(Joints::Name::waist,
                                        this->parent.waistAngle);
  this->parent.currentState = this;
  this->parent.hardware.eyes.crossFade(
      this->parent.currentEyeColour, this->eyeColour, this->eyeTransitionTime);
  this->parent.currentEyeColour = this->eyeColour;
}

char const* TrackingState::State::name()
{
  return this->stateName.c_str();
}

//////////////////////////////////////////////////////////////////////
// TooCloseState
//////////////////////////////////////////////////////////////////////

TrackingState::TooCloseState::TooCloseState(TrackingState& parent)
    : State(parent, "TooCloseState", Eyes::Colour::off, EYE_TRANSITION_TIME)
{
}

void TrackingState::TooCloseState::enter()
{
  BodyMotion::setArms(this->parent.hardware.joints, -30);
  State::enter();
}

void TrackingState::TooCloseState::runOnce()
{
  LOG_INFO("runOncening %s::%s", this->parent.name(), this->name());
}

//////////////////////////////////////////////////////////////////////
// WithinRangeState
//////////////////////////////////////////////////////////////////////

TrackingState::WithinRangeState::WithinRangeState(TrackingState& parent)
    : State(parent, "WithinRangeState", Eyes::Colour::red, EYE_TRANSITION_TIME)
{
}

void TrackingState::WithinRangeState::enter()
{
  State::enter();
}

void TrackingState::WithinRangeState::runOnce()
{
  LOG_INFO("runOncening %s::%s", this->parent.name(), this->name());
}

//////////////////////////////////////////////////////////////////////
// OutOfRangeState
//////////////////////////////////////////////////////////////////////

TrackingState::OutOfRangeState::OutOfRangeState(TrackingState& parent)
    : State(parent,
            "OutOfRangeState",
            Eyes::Colour::light_blue,
            EYE_TRANSITION_TIME)
{
}

void TrackingState::OutOfRangeState::enter()
{
  State::enter();
}

void TrackingState::OutOfRangeState::runOnce()
{
  LOG_INFO("runOncening %s::%s", this->parent.name(), this->name());
}