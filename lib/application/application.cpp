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
    if (this->hardware->modeSwitch.getState() == Switch::State::on)
    {
      this->danceState->enter();
    }
    else
    {
      this->trackingState->enter();
    }
    delay(500);
  }
}
