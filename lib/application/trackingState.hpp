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
};