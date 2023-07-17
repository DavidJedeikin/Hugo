#pragma once
#include <string>

namespace hugo
{

class SerialLogger
{
 public:
  void log(std::string&& message);
  static SerialLogger& getInstance();

  SerialLogger(SerialLogger const&) = delete;
  SerialLogger(SerialLogger&&) = delete;
  void operator=(SerialLogger const&) = delete;
  void operator=(SerialLogger&&) = delete;

 private:
  SerialLogger();
  static SerialLogger instance;
};

} // namespace hugo