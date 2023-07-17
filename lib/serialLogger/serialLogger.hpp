#pragma once
#include <string>

namespace hugo
{

class SerialLogger
{
 public:
  void log(std::string&& message);
  static SerialLogger& getInstance();
  SerialLogger();

 private:
  static SerialLogger instance;
};

} // namespace hugo