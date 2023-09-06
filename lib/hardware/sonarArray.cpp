#include "sonarArray.hpp"
#include "Arduino.h"
#include "log.hpp"

SonarArray::SonarArray()
{
  pinMode(this->rightSensor.triggerPin, OUTPUT);
  pinMode(this->rightSensor.echoPin, INPUT);
  pinMode(this->leftSensor.triggerPin, OUTPUT);
  pinMode(this->leftSensor.echoPin, INPUT);
}

SonarArray::Distance SonarArray::getDistance()
{
  this->distance.right = getDistance(this->rightSensor);
  delay(30);
  this->distance.left = getDistance(this->leftSensor);
  this->distance.min = std::min(this->distance.right, this->distance.left);
  return this->distance;
}

float SonarArray::getDistance(Hcsr04Sensor sensor)
{
  digitalWrite(sensor.triggerPin, LOW);
  delayMicroseconds(2);
  digitalWrite(sensor.triggerPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(sensor.triggerPin, LOW);
  return (pulseIn(sensor.echoPin, HIGH) / 2.0F) * MICROSECONDS_TO_SECONDS *
         SPEED_OF_SOUND * METERS_TO_CM;
}

std::string SonarArray::Distance::toString() const
{
  return format(
      "Right: %.0f, Left: %.0f Min: %.0f", this->right, this->left, this->min);
}
