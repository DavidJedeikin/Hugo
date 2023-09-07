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
  bool inSafeMode{false};

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

  Eyes::Colour currentEyeColour;
  ObjectState currentObjectState;

  Hardware& hardware;

  static constexpr float MIN_DISTANCE_CM{50};
  static constexpr float DETECTION_DISTANCE_CM{130};

  LinearMap::Params distanceToDanceSpeedParams{.inputMin = MIN_DISTANCE_CM,
                                               .inputMax =
                                                   DETECTION_DISTANCE_CM,
                                               .outputMin = 1500,
                                               .outputMax = 3500};
  LinearMap distanceToDanceSpeedMap;

  void updateCurrentObjectState();
};