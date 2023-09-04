#include "eyes.hpp"

Eyes::Eyes() : eyes(redPin, greenPin, bluePin, RGBLed::COMMON_ANODE)
{
}

void Eyes::setColour(Colour colour)
{
  switch (colour)
  {
    case Colour::red:
      this->eyes.setColor(RGBLed::RED);
      break;
    case Colour::green:
      this->eyes.setColor(RGBLed::GREEN);
      break;
    case Colour::blue:
      this->eyes.setColor(RGBLed::BLUE);
      break;
    case Colour::magenta:
      this->eyes.setColor(RGBLed::MAGENTA);
      break;
    case Colour::none:
      this->eyes.setColor(this->RGB_NO_COLOUR.data());
      break;
    case Colour::light_blue:
      this->eyes.setColor(this->RGB_LIGHT_BLUE.data());
      break;
  }
}
