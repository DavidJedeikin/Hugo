#pragma once

class IState
{
 public:
  virtual ~IState() = default;
  virtual void enter() = 0;
  virtual void runOnce() = 0;
  virtual char const* name() = 0;
};
