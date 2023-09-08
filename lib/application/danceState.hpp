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
  float objectDistance{0};
  Hardware& hardware;
  Eyes::Colour currentEyeColour{Eyes::Colour::light_blue};
  static constexpr uint32_t EYE_TRANSITION_TIME{500};

  //////////////////////////////////////////////////////////////////////
  // Distance Params
  //////////////////////////////////////////////////////////////////////
  static constexpr float MIN_DISTANCE_CM{20};
  static constexpr float MAX_DISTANCE_CM{50};
  LinearMap::Params distanceToDanceSpeedParams{.inputMin = MIN_DISTANCE_CM,
                                               .inputMax = MAX_DISTANCE_CM,
                                               .outputMin = 1500,
                                               .outputMax = 3500};
  LinearMap distanceToDanceSpeedMap;

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

   private:
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
  } withinRangeState;

  class OutOfRangeState : public State
  {
   public:
    OutOfRangeState(DanceState& parent);
    void enter() override;
    void runOnce() override;

  } outOfRangeState;
};