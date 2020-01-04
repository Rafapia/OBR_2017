/*
  OBR_PID.cpp - PID control class definition.
  Created by Rafael Piacsek, June 21, 2018.
  Private code, for OBR use only.
*/


#include "Arduino.h"
#include "OBR_PID.h"


// ##################################################################################################################################

// Define Contructor method.
OBR_PID::OBR_PID(float kP, float kI, float kD) {

  // Store tuning values.
  _kP = kP;
  _kI = kI;
  _kD = kD;

  Serial.println("PID initiated.");
}

// ##################################################################################################################################

// Define compute method.
int OBR_PID::compute(int error) {

  // Calculate time passed since last iteration.
  unsigned long now = millis();
  int dTime = now - _lastTime;

  // Compute all working error variables.
  _errorSum += (error * dTime);
  int dError = (error - _lastError) / dTime;

  // Calculate PID output.
  int output = (_kP * error) + (_kI * _errorSum) + (_kD * dError); 

  // Refresh variables for next iteration.
  _lastError = error;
  _lastTime = now;

  return output;
}
