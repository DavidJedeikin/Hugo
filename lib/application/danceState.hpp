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

  class TooCloseState : public IState
  {
   public:
    TooCloseState(DanceState& parent);
    void enter() override;
    void runOnce() override;
    char const* name() override;

   private:
    DanceState& parent;
    Eyes::Colour eyeColour{Eyes::Colour::green};

  } tooCloseState;

  class WithinRangeState : public IState
  {
   public:
    WithinRangeState(DanceState& parent);
    void enter() override;
    void runOnce() override;
    char const* name() override;

   private:
    DanceState& parent;
    Eyes::Colour eyeColour{Eyes::Colour::red};

  } withinRangeState;

  class OutOfRangeState : public IState
  {
   public:
    OutOfRangeState(DanceState& parent);
    void enter() override;
    void runOnce() override;
    char const* name() override;

   private:
    DanceState& parent;
    Eyes::Colour eyeColour{Eyes::Colour::light_blue};

  } outOfRangeState;
};