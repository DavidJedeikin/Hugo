#pragma once

class LinearFirstOrderFiler
{
 public:
  LinearFirstOrderFiler(float alpha);
  float getVal(float measurement);

 private:
  float val{0};
  float alpha{0};
};