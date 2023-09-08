#pragma once
#include "hardware.hpp"
#include "iState.hpp"

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

  static constexpr float ARM_ANGLE{-30};
  static constexpr float MIN_DISTANCE_CM{35};
  static constexpr float MAX_DISTANCE_CM{100};

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

   private:
  } withinRangeState;

  class OutOfRangeState : public State
  {
   public:
    OutOfRangeState(TrackingState& parent);
    void enter() override;
    void runOnce() override;

  } outOfRangeState;
};