#pragma once
#include "Arduino.h"
#include <RGBLed.h>
#include <array>

class Eyes
{
 public:
  enum class Colour
  {
    none,
    red,
    green,
    blue,
    light_blue,
    magenta
  };

  Eyes();
  void setColour(Colour colour);

 private:
  RGBLed eyes;

  int redPin{A0};
  int greenPin{A1};
  int bluePin{A2};

  inline static std::array<int, 3> RGB_NO_COLOUR{0, 0, 0};
  inline static std::array<int, 3> RGB_LIGHT_BLUE{173, 216, 255};
};
