#include "bodyMotion.hpp"
#include "log.hpp"
#include <algorithm>
#include <cmath>

namespace BodyMotion
{

void minFullSweep(Joints& joints, int milliSeconds, int armOffset)
{
  // This is not generic - I know the waist range is within the arm ranges
  Joints::Limits waistLimits = joints.getLimits(Joints::Name::waist);
  int waistRange = joints.getLimitsRange(Joints::Name::waist);
  uint32_t delayFor = static_cast<uint32_t>(milliSeconds / waistRange) / 2;

  for (int i = 0; i < waistLimits.maxAngle; i++)
  {
    joints.setAngle(Joints::Name::waist, i);
    joints.setAngle(Joints::Name::left_shoulder, i + armOffset);
    joints.setAngle(Joints::Name::right_shoulder, -i + armOffset);
    delay(delayFor);
  }
  for (int i = waistLimits.maxAngle; i > waistLimits.minAngle; i--)
  {
    joints.setAngle(Joints::Name::waist, i);
    joints.setAngle(Joints::Name::left_shoulder, i + armOffset);
    joints.setAngle(Joints::Name::right_shoulder, -i + armOffset);
    delay(delayFor);
  }
  for (int i = waistLimits.minAngle; i < 0; i++)
  {
    joints.setAngle(Joints::Name::waist, i);
    joints.setAngle(Joints::Name::left_shoulder, i + armOffset);
    joints.setAngle(Joints::Name::right_shoulder, -i + armOffset);
    delay(delayFor);
  }
}

void moveArms(Joints& joints, int start, int end, int milliSeconds)
{
  int range = std::fabs(end - start);
  uint32_t delayFor = static_cast<uint32_t>(milliSeconds / range);
  if (start < end)
  {
    for (int i = start; i < end; i++)
    {
      joints.setAngle(Joints::Name::left_shoulder, i);
      joints.setAngle(Joints::Name::right_shoulder, i);
      delay(delayFor);
    }
  }
  else
  {
    for (int i = start; i > end; i--)
    {
      joints.setAngle(Joints::Name::left_shoulder, i);
      joints.setAngle(Joints::Name::right_shoulder, i);
      delay(delayFor);
    }
  }
}
} // namespace BodyMotion