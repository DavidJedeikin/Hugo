#include "serialLogger.hpp"
#include <Arduino.h>

namespace hugo
{

// SerialLogger is a singleton
SerialLogger SerialLogger::instance = SerialLogger();

SerialLogger::SerialLogger()
{
  Serial.begin(115200);
}

void SerialLogger::log(std::string&& message)
{
  Serial.printf("%s\n\r", message.c_str());
}

SerialLogger& SerialLogger::getInstance()
{
  return SerialLogger::instance;
}

} // namespace hugo