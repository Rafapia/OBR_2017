/*
  OBR_PID.h - PID control class declaration.
  Created by Rafael Piacsek, June 21, 2018.
  Private code, for OBR use only.
*/

#ifndef OBR_PID_h
#define OBR_PID_h

#include "Arduino.h"


class OBR_PID {

  private:

    // Tuning variables.
    float _kP, _kI, _kD;

    // Working variables.
    unsigned long _lastTime=0;
    int _lastError, _errorSum;

  public:

    // Constructor method.
    OBR_PID(float Kp, float kI, float kD);

    // Methods.
    int compute(int error);
   
};

#endif
