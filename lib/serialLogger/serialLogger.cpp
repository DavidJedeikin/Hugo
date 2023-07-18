#include "serialLogger.hpp"
#include <Arduino.h>

namespace hugo
{
void SerialLogger::init()
{
  Serial.begin(115200);
}

SerialLogger SerialLogger::instance;

void SerialLogger::log(std::string&& message)
{
  Serial.printf("%s\n\r", message.c_str());
}

SerialLogger& SerialLogger::getInstance()
{
  return SerialLogger::instance;
}

} // namespace hugo