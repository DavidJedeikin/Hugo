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

  //////////////////////////////////////////////////////////////////////
  // Control
  //////////////////////////////////////////////////////////////////////
  static constexpr float MAX_ANGLE_CHANGE{5};
  PidController::Parameters pidParams{.Kp = 1,
                                      .Kd = 0,
                                      .Ki = 0,
                                      .timestepMs = 50,
                                      .maxControlSignal = MAX_ANGLE_CHANGE,
                                      .minControlSignal = -MAX_ANGLE_CHANGE};

  PidController pidController;

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