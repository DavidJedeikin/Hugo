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

  LinearMap::Params distanceToDanceSpeedParams{
      .inputMin = 55, .inputMax = 180, .outputMin = 1000, .outputMax = 2000};
  LinearMap distanceToDanceSpeedMap;
};