#pragma once
#include "Arduino.h"
#include <RGBLed.h>
#include <array>

class Eyes
{
 public:
  enum class Colour
  {
    off,
    red,
    green,
    blue,
    light_blue,
    magenta
  };

  Eyes();
  void setColour(Colour colour);
  void crossFade(Colour from, Colour to, int milliSeconds);

 private:
  int redPin{A0};
  int greenPin{A1};
  int bluePin{A2};

  RGBLed eyes;

  std::array<int, 3> getRgb(Colour colour);
  inline static std::array<int, 3> RGB_OFF{0, 0, 0};
  inline static std::array<int, 3> RGB_LIGHT_BLUE{173, 216, 255};

  std::array<int, 3> stdArrayFromCStyleRgb(int (&rgb)[3]);
};
