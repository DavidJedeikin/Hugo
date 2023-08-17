
#include "pidController.hpp"
#include "log.hpp"
#include <algorithm>

namespace core
{

PidController::PidController(Parameters params)
    : params(params),
      timestepSeconds(static_cast<float>(params.timestepMs) / 1000.0F)
{
}

void PidController::updateKp(float Kp)
{
  this->params.Kp = Kp;
}

void PidController::updateKd(float Kd)
{
  this->params.Kd = Kd;
}

void PidController::updateKi(float Ki)
{
  if (Ki > this->params.Ki && this->errorIntegral != 0.0)
  {
    float increaseRatio = Ki / this->params.Ki;
    this->errorIntegral /= increaseRatio;
  }
  else if (Ki == 0.0)
  {
    this->errorIntegral = 0.0;
  }

  this->params.Ki = Ki;
}

float PidController::getControlSignal(float currentState, float setpoint)
{
  float currentError = setpoint - currentState;

  float errorDerivative =
      (currentError - previousError) / this->timestepSeconds;

  this->errorIntegral += (currentError * this->timestepSeconds);

  // Add integral windup mechanism
  this->errorIntegral = std::clamp<float>(
      this->errorIntegral,
      this->params.windupLimitFactor * params.minControlSignal / params.Ki,
      this->params.windupLimitFactor * params.maxControlSignal / params.Ki);

  float controlSignal = (params.Kp * currentError) +
                        (params.Kd * errorDerivative) +
                        (params.Ki * this->errorIntegral);

  this->previousError = currentError;

  // Clamp the output
  return std::clamp<float>(
      controlSignal, params.minControlSignal, params.maxControlSignal);
}

std::string PidController::Parameters::toString() const
{
  return format(
      "PID Control Parameters \n Kp: %.2f, Kd: %.2f, Ki: %.2f, dtMs: %.2f, "
      "Min: %.2f, Max: %.2f, Windup Factor:  %.2f",
      this->Kp,
      this->Kd,
      this->Ki,
      this->timestepMs,
      this->minControlSignal,
      this->maxControlSignal,
      this->windupLimitFactor);
}

} // namespace core
