#include "linearFirstOrderFilter.hpp"

LinearFirstOrderFiler::LinearFirstOrderFiler(float alpha) : alpha(alpha){};

float LinearFirstOrderFiler::getVal(float measurement)
{
  float nextVal = (this->alpha * measurement) + ((1 - this->alpha) * this->val);
  this->val = nextVal;
  return this->val;
}
