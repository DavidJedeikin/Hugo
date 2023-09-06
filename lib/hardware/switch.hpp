#pragma once
#include "Arduino.h"
#include <string>

class Switch
{
 public:
  enum class State
  {
    on,
    off
  };
  Switch();
  State getState();
  std::string toString(State state);

 private:
  int switchPin{A3};
};
