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