
#include "pidController.hpp"
#include "log.hpp"
#include <algorithm>

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
  this->errorIntegral =
      std::clamp<float>(this->errorIntegral,
                        this->params.windupLimitFactor *
                            this->params.minControlSignal / this->params.Ki,
                        this->params.windupLimitFactor *
                            this->params.maxControlSignal / this->params.Ki);

  float controlSignal = (this->params.Kp * currentError) +
                        (this->params.Kd * errorDerivative) +
                        (this->params.Ki * this->errorIntegral);

  this->previousError = currentError;

  LOG_RAW("CurrentState: %.2f, Setpoint: %.2f, CurrentError: %.2f, "
          "ControlSignal: %.2f, Kp: %.2f, ExpectedControlSignal: %.2f, "
          "ErrorIntegral: %.2f",
          currentState,
          setpoint,
          currentError,
          controlSignal,
          this->params.Kp,
          this->params.Kp * currentError + this->params.Kd * errorDerivative +
              this->params.Ki * this->errorIntegral,
          this->errorIntegral);

  // Clamp the output
  return std::clamp<float>(
      controlSignal, params.minControlSignal, params.maxControlSignal);
}

std::string PidController::Parameters::toString() const
{
  return format(
      "PID Control Parameters \n Kp: %.2f, Kd: %.2f, Ki: %.2f, dtMs: %u, "
      "Min: %.2f, Max: %.2f, Windup Factor:  %.2f",
      this->Kp,
      this->Kd,
      this->Ki,
      this->timestepMs,
      this->minControlSignal,
      this->maxControlSignal,
      this->windupLimitFactor);
}
