#pragma once
#include "danceState.hpp"
#include "eyes.hpp"
#include "iState.hpp"
#include "trackingState.hpp"
#include <memory>

class Application
{
 public:
  Application();
  void run();

 private:
  std::unique_ptr<Hardware> hardware;

  IState* currentState{nullptr};
  std::unique_ptr<IState> danceState{nullptr};
  std::unique_ptr<IState> trackingState{nullptr};

  IState* getDesiredState();
};

#include "Arduino.h"
#include "application.hpp"
#include "log.hpp"
#include "serialLogger.hpp"

Application::Application()
{
  // Having to call this is preventing you from being able to use the
  // initializer list
  SerialLogger::getInstance().init();
  LOG_INFO("%s", "Starting Application!");

  this->hardware = std::make_unique<Hardware>();
  this->danceState = std::make_unique<DanceState>(*this->hardware);
  this->trackingState = std::make_unique<TrackingState>(*this->hardware);

  this->currentState = this->danceState.get();

  this->hardware->eyes.setColour(Eyes::Colour::light_blue);
}

void Application::run()
{
  while (true)
  {
    auto desiredState = this->getDesiredState();
    if (desiredState == nullptr)
    {
      LOG_WARN("%s", "Desired state is a nullptr");
      return;
    }

    if (this->currentState != desiredState)
    {
      this->currentState = desiredState;
      this->currentState->enter();
    }
    else
    {
      this->currentState->runOnce();
    }
  }
}

IState* Application::getDesiredState()
{
  // In the future you could use whatever signal to select the desired state
  return this->hardware->modeSwitch.getState() == Switch::State::on
             ? this->danceState.get()
             : this->trackingState.get();
}

#pragma once
#include "hardware.hpp"
#include "iState.hpp"
#include "pidController.hpp"

class TrackingState : public IState
{
 public:
  TrackingState(Hardware& hardware);
  void enter() override;
  void runOnce() override;
  char const* name() override;

 private:
  Hardware& hardware;
  int waistAngle{0};
  float objectDistance{0};
  Eyes::Colour currentEyeColour{Eyes::Colour::light_blue};
  static constexpr uint32_t EYE_TRANSITION_TIME{500};

  static constexpr float ARM_ANGLE{-50};
  static constexpr float MIN_DISTANCE_CM{15};
  static constexpr float MAX_DISTANCE_CM{150};

  void setWaistAngle(int angle);

  //////////////////////////////////////////////////////////////////////
  // Control
  //////////////////////////////////////////////////////////////////////

  static constexpr float MAX_ANGLE_CHANGE{10};
  PidController::Parameters pidParams{.Kp = 1.0,
                                      .Kd = 0.0,
                                      .Ki = 0,
                                      .timestepMs = 50,
                                      .maxControlSignal = MAX_ANGLE_CHANGE,
                                      .minControlSignal = -MAX_ANGLE_CHANGE};

  PidController pidController;
  Joints::Limits waistLimits;

  //////////////////////////////////////////////////////////////////////
  // States
  //////////////////////////////////////////////////////////////////////

  IState* currentState{nullptr};

  IState* getDesiredState();

  class State : public IState
  {
   public:
    State(TrackingState& parent,
          std::string&& stateName,
          Eyes::Colour eyeColour,
          int eyeTransitionTime);
    void enter() override;
    char const* name() override;

   protected:
    TrackingState& parent;
    std::string stateName;
    Eyes::Colour eyeColour;
    int eyeTransitionTime;
  };

  class TooCloseState : public State

  {
   public:
    TooCloseState(TrackingState& parent);
    void enter() override;
    void runOnce() override;
  } tooCloseState;

  class WithinRangeState : public State
  {
   public:
    WithinRangeState(TrackingState& parent);
    void enter() override;
    void runOnce() override;
  } withinRangeState;

  class OutOfRangeState : public State
  {
   public:
    OutOfRangeState(TrackingState& parent);
    void enter() override;
    void runOnce() override;

  } outOfRangeState;
};

#include "bodyMotion.hpp"
#include "log.hpp"
#include "trackingState.hpp"

TrackingState::TrackingState(Hardware& hardware)
    : hardware(hardware), pidController(this->pidParams),
      waistLimits(this->hardware.joints.getLimits(Joints::Name::waist)),
      tooCloseState(*this), withinRangeState(*this), outOfRangeState(*this)
{
  this->outOfRangeState.enter();
}

void TrackingState::enter()
{
  LOG_INFO("Entering the %s", this->name());
  this->hardware.eyes.setColour(Eyes::Colour::green);
  BodyMotion::setArms(this->hardware.joints, ARM_ANGLE);
  this->setWaistAngle(0);
}

void TrackingState::runOnce()
{
  auto desiredState = this->getDesiredState();
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

void TrackingState::setWaistAngle(int angle)
{
  if (angle < this->waistLimits.minAngle || angle > this->waistLimits.maxAngle)
  {
    LOG_WARN("Attempted to set a waist angle: %d out of bounds: [%d, %d] - "
             "clamping at: %d",
             angle,
             this->waistLimits.minAngle,
             this->waistLimits.maxAngle,
             angle < this->waistLimits.minAngle ? this->waistLimits.minAngle
                                                : this->waistLimits.maxAngle);
    this->waistAngle = std::clamp(
        angle, this->waistLimits.minAngle, this->waistLimits.maxAngle);
  }
  this->hardware.joints.setAngle(Joints::Name::waist, this->waistAngle);
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
  State::enter();
}

void TrackingState::TooCloseState::runOnce()
{
  LOG_INFO("Running once %s::%s", this->parent.name(), this->name());
  LOG_INFO("Distance: %.2f, Min: %.2f, Max: %.2f",
           this->parent.objectDistance,
           MIN_DISTANCE_CM,
           MAX_DISTANCE_CM);
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

  SonarArray::Distance distance =
      this->parent.hardware.sonarArray.getDistance();

  float difference = distance.right - distance.left;

  int controlSignal = static_cast<int>(
      this->parent.pidController.getControlSignal(difference, 0));
  if (std::fabs(difference) > 5)
  {
    this->parent.waistAngle += controlSignal;
  }

  this->parent.setWaistAngle(this->parent.waistAngle);

  LOG_INFO("Difference: %.2f, ControlSignal: %d, WaistAngle: %d",
           difference,
           controlSignal,
           this->parent.waistAngle);
  delay(this->parent.pidParams.timestepMs);
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
  LOG_INFO("Running once %s::%s", this->parent.name(), this->name());
  LOG_INFO("Distance: %.2f, Min: %.2f, Max: %.2f",
           this->parent.objectDistance,
           MIN_DISTANCE_CM,
           MAX_DISTANCE_CM);
}

#pragma once
#include "hardware.hpp"
#include "iState.hpp"
#include "linearMap.hpp"

class DanceState : public IState
{
 public:
  DanceState(Hardware& hardware);
  void enter() override;
  void runOnce() override;
  char const* name() override;

 private:
  Hardware& hardware;
  float objectDistance{0};
  Eyes::Colour currentEyeColour{Eyes::Colour::light_blue};
  static constexpr uint32_t EYE_TRANSITION_TIME{500};

  //////////////////////////////////////////////////////////////////////
  // Motion Params
  //////////////////////////////////////////////////////////////////////
  float armMotionOffset{-15};

  //////////////////////////////////////////////////////////////////////
  // Distance Params
  //////////////////////////////////////////////////////////////////////
  static constexpr float MIN_DISTANCE_CM{35};
  static constexpr float MAX_DISTANCE_CM{100};
  static constexpr LinearMap::Params distanceToSpeedParams{
      .inputMin = MIN_DISTANCE_CM,
      .inputMax = MAX_DISTANCE_CM,
      .outputMin = 500,
      .outputMax = 3000};
  LinearMap distanceToSpeed;

  //////////////////////////////////////////////////////////////////////
  // States
  //////////////////////////////////////////////////////////////////////

  IState* currentState{nullptr};
  IState* getDesiredState();

  class State : public IState
  {
   public:
    State(DanceState& parent,
          std::string&& stateName,
          Eyes::Colour eyeColour,
          int eyeTransitionTime);
    void enter() override;
    char const* name() override;

   protected:
    DanceState& parent;
    std::string stateName;
    Eyes::Colour eyeColour;
    int eyeTransitionTime;
  };

  class TooCloseState : public State

  {
   public:
    TooCloseState(DanceState& parent);
    void enter() override;
    void runOnce() override;
  } tooCloseState;

  class WithinRangeState : public State
  {
   public:
    WithinRangeState(DanceState& parent);
    void enter() override;
    void runOnce() override;

   private:
    int danceSpeed{
        static_cast<int>(DanceState::distanceToSpeedParams.outputMax)};
  } withinRangeState;

  class OutOfRangeState : public State
  {
   public:
    OutOfRangeState(DanceState& parent);
    void enter() override;
    void runOnce() override;

  } outOfRangeState;
};

#include "bodyMotion.hpp"
#include "danceState.hpp"
#include "log.hpp"

DanceState::DanceState(Hardware& hardware)
    : hardware(hardware), distanceToSpeed(distanceToSpeedParams),
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
// Base State
//////////////////////////////////////////////////////////////////////

DanceState::State::State(DanceState& parent,
                         std::string&& stateName,
                         Eyes::Colour eyeColour,
                         int eyeTransitionTime)
    : parent(parent), stateName(stateName), eyeColour(eyeColour),
      eyeTransitionTime(eyeTransitionTime)
{
}

void DanceState::State::enter()
{
  LOG_INFO("Entering %s::%s", this->parent.name(), this->name());
  this->parent.currentState = this;
  this->parent.hardware.eyes.crossFade(
      this->parent.currentEyeColour, this->eyeColour, this->eyeTransitionTime);
  this->parent.currentEyeColour = this->eyeColour;
}

char const* DanceState::State::name()
{
  return this->stateName.c_str();
}

//////////////////////////////////////////////////////////////////////
// TooCloseState
//////////////////////////////////////////////////////////////////////

DanceState::TooCloseState::TooCloseState(DanceState& parent)
    : State(parent, "TooCloseState", Eyes::Colour::off, EYE_TRANSITION_TIME)
{
}

void DanceState::TooCloseState::enter()
{
  BodyMotion::setArms(this->parent.hardware.joints, -30);
  State::enter();
}

void DanceState::TooCloseState::runOnce()
{
  LOG_INFO("Running %s::%s", this->parent.name(), this->name());
}

//////////////////////////////////////////////////////////////////////
// WithinRangeState
//////////////////////////////////////////////////////////////////////

DanceState::WithinRangeState::WithinRangeState(DanceState& parent)
    : State(parent, "WithinRangeState", Eyes::Colour::red, EYE_TRANSITION_TIME)
{
}

void DanceState::WithinRangeState::enter()
{
  State::enter();
  for (int i = 0; i < 3; i++)
  {
    this->parent.hardware.eyes.crossFade(
        Eyes::Colour::blue, Eyes::Colour::red, 100);
    this->parent.hardware.eyes.crossFade(
        Eyes::Colour::red, Eyes::Colour::red, 100);
  }
}

void DanceState::WithinRangeState::runOnce()
{
  LOG_INFO("Running %s::%s", this->parent.name(), this->name());
  this->danceSpeed =
      this->parent.distanceToSpeed.getOutput(this->parent.objectDistance);
  BodyMotion::minFullSweep(this->parent.hardware.joints,
                           this->danceSpeed,
                           this->parent.armMotionOffset);
}

//////////////////////////////////////////////////////////////////////
// OutOfRangeState
//////////////////////////////////////////////////////////////////////

DanceState::OutOfRangeState::OutOfRangeState(DanceState& parent)
    : State(parent,
            "OutOfRangeState",
            Eyes::Colour::light_blue,
            EYE_TRANSITION_TIME)
{
}

void DanceState::OutOfRangeState::enter()
{
  State::enter();
}

void DanceState::OutOfRangeState::runOnce()
{
  LOG_INFO("Running %s::%s", this->parent.name(), this->name());
}

#pragma once
#include <string>

class SonarArray
{
 public:
  struct Distance
  {
    float right;
    float left;
    float min;
    [[nodiscard]] std::string toString() const;
  };

  SonarArray();
  Distance getDistance();

 private:
  Distance distance;

  struct Hcsr04Sensor
  {
    int triggerPin;
    int echoPin;
  };

  Hcsr04Sensor rightSensor{.triggerPin = 7, .echoPin = 11};
  Hcsr04Sensor leftSensor{.triggerPin = 16, .echoPin = 15};

  static constexpr float METERS_TO_CM{100.0F};
  static constexpr float SPEED_OF_SOUND{343.0F};
  static constexpr float MICROSECONDS_TO_SECONDS{1 / 1000000.0F};

  static float getDistance(Hcsr04Sensor sensor);
};

#include "Arduino.h"
#include "log.hpp"
#include "sonarArray.hpp"

SonarArray::SonarArray()
{
  pinMode(this->rightSensor.triggerPin, OUTPUT);
  pinMode(this->rightSensor.echoPin, INPUT);
  pinMode(this->leftSensor.triggerPin, OUTPUT);
  pinMode(this->leftSensor.echoPin, INPUT);
}

SonarArray::Distance SonarArray::getDistance()
{
  this->distance.right = getDistance(this->rightSensor);
  delay(30);
  this->distance.left = getDistance(this->leftSensor);
  this->distance.min = std::min(this->distance.right, this->distance.left);
  return this->distance;
}

float SonarArray::getDistance(Hcsr04Sensor sensor)
{
  digitalWrite(sensor.triggerPin, LOW);
  delayMicroseconds(2);
  digitalWrite(sensor.triggerPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(sensor.triggerPin, LOW);
  return (pulseIn(sensor.echoPin, HIGH) / 2.0F) * MICROSECONDS_TO_SECONDS *
         SPEED_OF_SOUND * METERS_TO_CM;
}

std::string SonarArray::Distance::toString() const
{
  return format(
      "Right: %.0f, Left: %.0f Min: %.0f", this->right, this->left, this->min);
}
