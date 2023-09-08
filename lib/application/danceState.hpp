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