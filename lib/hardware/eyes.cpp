#include "eyes.hpp"

Eyes::Eyes() : led(redPin, greenPin, bluePin, RGBLed::COMMON_ANODE)
{
}

void Eyes::setColour(Colour colour)
{
  switch (colour)
  {
    case Colour::red:
      this->led.setColor(RGBLed::RED);
      break;
    case Colour::green:
      this->led.setColor(RGBLed::GREEN);
      break;
    case Colour::blue:
      this->led.setColor(RGBLed::BLUE);
      break;
    case Colour::magenta:
      this->led.setColor(RGBLed::MAGENTA);
      break;
    case Colour::none:
      this->led.setColor(this->RGB_NO_COLOUR.data());
      break;
    case Colour::light_blue:
      this->led.setColor(this->RGB_LIGHT_BLUE.data());
      break;
  }
}
