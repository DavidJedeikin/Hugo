#pragma once
#include <string>

class SonarArray
{
 public:
  struct Distance
  {
    float right;
    float left;
    [[nodiscard]] std::string toString() const;
  };

  SonarArray();
  Distance getDistance();

 private:
  Distance distance;

  struct Hcsr04Sensor
  {
    int triggerPin;
    int echoPin;
  };

  Hcsr04Sensor rightSensor{.triggerPin = 7, .echoPin = 11};
  Hcsr04Sensor leftSensor{.triggerPin = 16, .echoPin = 15};

  static constexpr float METERS_TO_CM{100.0F};
  static constexpr float SPEED_OF_SOUND{343.0F};
  static constexpr float MICROSECONDS_TO_SECONDS{1 / 1000000.0F};

  static float getDistance(Hcsr04Sensor sensor);
};