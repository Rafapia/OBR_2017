/*
  OBR_Gyro.h - Gyro control class header.
  Created by Rafael Piacsek, June 24, 2018.
  Private code, for OBR use only.
*/

#ifndef OBR_Gyro_h
#define OBR_Gyro_h

#include "Arduino.h"
#include <Adafruit_BNO055.h>


class OBR_Gyro {

  private:

    float _lastAngle = 0;
    Adafruit_BNO055 bno = Adafruit_BNO055();

  public:

    OBR_Gyro();

    int getSensorAngle();
    int getAngle(int targetAngle);
    void resetAngle();
  
};

#endif

