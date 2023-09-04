#pragma once

class LinearMap
{
 public:
  struct Params
  {
    float inputMin;
    float inputMax;
    float outputMin;
    float outputMax;
  };

  LinearMap(Params params);

  float getOutput(float input);

 private:
  float m;
  float c;
};