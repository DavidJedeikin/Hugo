#include "serialLogger.hpp"
#include <Arduino.h>

// Come back to this, really seems overkill

void SerialLogger::init()
{
  Serial.begin(115200);
}

SerialLogger SerialLogger::instance;

void SerialLogger::log(std::string&& message)
{
  Serial.printf("%s\r\n", message.c_str());
}

SerialLogger& SerialLogger::getInstance()
{
  return SerialLogger::instance;
}
