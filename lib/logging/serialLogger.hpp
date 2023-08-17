#pragma once
#include <string>

namespace hugo
{

class SerialLogger
{
 public:
  void init();
  void log(std::string&& message);
  static SerialLogger& getInstance();

  SerialLogger(SerialLogger const&) = delete;
  SerialLogger(SerialLogger&&) = delete;
  void operator=(SerialLogger const&) = delete;
  void operator=(SerialLogger&&) = delete;

 private:
  SerialLogger() = default;
  static SerialLogger instance;
};

} // namespace hugo