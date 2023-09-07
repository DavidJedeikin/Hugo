#include "eyes.hpp"

Eyes::Eyes() : eyes(redPin, greenPin, bluePin, RGBLed::COMMON_ANODE)
{
}

void Eyes::setColour(Colour colour)
{
  this->eyes.setColor(this->getRgb(colour).data());
}

void Eyes::crossFade(Colour from, Colour to, int milliSeconds)
{
  this->eyes.crossFade(
      this->getRgb(from).data(), this->getRgb(to).data(), 100, milliSeconds);
}

std::array<int, 3> Eyes::getRgb(Colour colour)
{
  switch (colour)
  {
    case Colour::red:
      return this->stdArrayFromCStyleRgb(RGBLed::RED);
    case Colour::green:
      return this->stdArrayFromCStyleRgb(RGBLed::GREEN);
    case Colour::blue:
      return this->stdArrayFromCStyleRgb(RGBLed::BLUE);
    case Colour::magenta:
      return this->stdArrayFromCStyleRgb(RGBLed::MAGENTA);
    case Colour::off:
      return this->RGB_OFF;
    case Colour::light_blue:
      return this->RGB_LIGHT_BLUE;
  }
}

std::array<int, 3> Eyes::stdArrayFromCStyleRgb(int (&rgb)[3])
{
  return {rgb[0], rgb[1], rgb[2]};
}
