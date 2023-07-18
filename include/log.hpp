#pragma once

#include "serialLogger.hpp"
#include <cstdio>
#include <functional>
#include <memory>
#include <sstream>
#include <string>
#include <type_traits>
#include <vector>

template <typename T> constexpr auto decayStrings(T arg)
{
  if constexpr (std::is_same<T, std::string>::value)
  {
    return arg.c_str();
  }
  else
  {
    return arg;
  }
}

template <typename... Args> std::string format(char const* format, Args... args)
{
  // Extra space for '\0'
  int size_s = snprintf(nullptr, 0, format, decayStrings(args)...) + 1;
  auto size = static_cast<size_t>(size_s);
  auto buf = std::make_unique<char[]>(size);
  snprintf(buf.get(), size, format, decayStrings(args)...);
  return {buf.get()};
}

#define LOG_RAW(fmt, ...)                                                      \
  hugo::SerialLogger::getInstance().log(format(fmt, ##__VA_ARGS__))
