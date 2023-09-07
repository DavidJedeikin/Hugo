#pragma once
#include "hardware.hpp"
#include "iState.hpp"
#include "linearMap.hpp"

class DanceState : public IState
{
 public:
  DanceState(Hardware& hardware);
  void enter() override;
  void runOnce() override;
  char const* name() override;

 private:
  enum class DetectionState
  {
    detected,
    still_detected,
    gone,
    still_gone
  };

  static std::string detectionStateToString(DetectionState state);

  struct ObjectState
  {
    float distance;
    DetectionState detectionState;
    std::string toString();
  };

  ObjectState currentObjectState;

  Hardware& hardware;

  static constexpr float DETECTION_DISTANCE_CM{20};

  LinearMap::Params distanceToDanceSpeedParams{
      .inputMin = 55, .inputMax = 180, .outputMin = 1000, .outputMax = 2000};
  LinearMap distanceToDanceSpeedMap;

  void updateCurrentObjectState();
};