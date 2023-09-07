#include "linearMap.hpp"
#include <algorithm>

LinearMap::LinearMap(Params params) : params(params)
{
  this->m = (params.outputMax - params.outputMin) /
            (params.inputMax - params.inputMin);
  this->c = params.outputMax - this->m * params.inputMax;
}

float LinearMap::getOutput(float input)
{
  // y = m * x + c
  return std::clamp(this->m * input + this->c,
                    this->params.outputMin,
                    this->params.outputMax);
}
