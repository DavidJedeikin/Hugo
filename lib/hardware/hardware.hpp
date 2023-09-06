#pragma once
#include "eyes.hpp"
#include "joints.hpp"
#include "sonarArray.hpp"
#include "switch.hpp"

struct Hardware
{
  Eyes eyes;
  Joints joints;
  SonarArray sonarArray;
  Switch modeSwitch;
};