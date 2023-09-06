#include "switch.hpp"

Switch::Switch()
{
  pinMode(this->switchPin, INPUT_PULLDOWN);
}

Switch::State Switch::getState()
{
  return digitalRead(this->switchPin) ? State::on : State::off;
}

std::string Switch::toString(State state)
{
  switch (state)
  {
    case State::on:
      return "ON";
    case State::off:
      return "OFF";
  }
}
