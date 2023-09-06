#include "application.hpp"
#include "Arduino.h"
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
