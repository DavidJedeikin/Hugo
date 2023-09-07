#include "danceState.hpp"
#include "bodyMotion.hpp"
#include "log.hpp"

DanceState::DanceState(Hardware& hardware)
    : hardware(hardware), distanceToDanceSpeedMap(distanceToDanceSpeedParams)
{
}

void DanceState::enter()
{
  LOG_INFO("Entering the %s", this->name());
  for (int i = 0; i < 5; i++)
  {
    this->hardware.eyes.setColour(Eyes::Colour::red);
    delay(100);
    this->hardware.eyes.setColour(Eyes::Colour::light_blue);
    delay(100);
  }
  delay(100);
}

void DanceState::runOnce()
{
  this->updateCurrentObjectState();
  LOG_INFO("%s", this->currentObjectState.toString());
  switch (this->currentObjectState.detectionState)
  {
    case DetectionState::detected: {
      this->hardware.eyes.crossFade(
          Eyes::Colour::light_blue, Eyes::Colour::red, 1000);
      break;
    }
    case DetectionState::still_detected: {
      int sweepSpeedMs =
          static_cast<int>(this->distanceToDanceSpeedMap.getOutput(
              this->currentObjectState.distance));
      // BodyMotion::minFullSweep(this->hardware.joints, sweepSpeedMs, -15);
      break;
    }
    case DetectionState::gone: {
      this->hardware.eyes.crossFade(
          Eyes::Colour::red, Eyes::Colour::light_blue, 1000);
      break;
    }
    case DetectionState::still_gone: {
      this->hardware.eyes.setColour(Eyes::Colour::light_blue);
      break;
    }
  }
}

char const* DanceState::name()
{
  return "DanceState";
}

std::string DanceState::detectionStateToString(DetectionState state)
{
  switch (state)
  {
    case DetectionState::detected:
      return "DETECTED";
    case DetectionState::still_detected:
      return "STILL_DETECTED";
    case DetectionState::gone:
      return "GONE";
    case DetectionState::still_gone:
      return "STILL_GONE";
  }
}

std::string DanceState::ObjectState::toString()
{
  return format("State: %s, Distance: %.2f",
                DanceState::detectionStateToString(this->detectionState),
                this->distance);
}

void DanceState::updateCurrentObjectState()
{
  float distance = this->hardware.sonarArray.getDistance().min;
  this->currentObjectState.distance = distance;

  bool objectInView = distance < DETECTION_DISTANCE_CM;

  auto detectionState = this->currentObjectState.detectionState;

  if (detectionState == DetectionState::detected ||
      detectionState == DetectionState::still_detected)
  {
    if (objectInView)
    {
      this->currentObjectState.detectionState = DetectionState::still_detected;
    }
    else
    {
      this->currentObjectState.detectionState = DetectionState::gone;
    }
    return;
  }

  // Getting here means it's either gone or still_gone
  if (!objectInView)
  {
    this->currentObjectState.detectionState = DetectionState::still_gone;
  }
  else
  {
    this->currentObjectState.detectionState = DetectionState::detected;
  }
}
