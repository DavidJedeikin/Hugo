#pragma once
#include "joints.hpp"

namespace BodyMotion
{

void minFullSweep(Joints& joints, int milliSeconds, int armOffset = 0);

void moveArms(Joints& joints, int start, int end, int milliSeconds);

} // namespace BodyMotion