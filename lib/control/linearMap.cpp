#include "linearMap.hpp"

LinearMap::LinearMap(Params params)
{
  this->m = (params.outputMax - params.outputMin) /
            (params.inputMax - params.inputMin);
  this->c = params.outputMax - this->m * params.inputMax;
}

float LinearMap::getOutput(float input)
{
  // y = m * x + c
  return this->m * input + this->c;
}
