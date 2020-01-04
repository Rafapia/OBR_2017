/*
  OBR_Gyro.cpp - Gyro control class definition.
  Created by Rafael Piacsek, June 24, 2018.
  Private code, for OBR use only.
*/


#include "Arduino.h"
#include "OBR_Gyro.h"


OBR_Gyro::OBR_Gyro() {

  bno.begin();
  bno.setExtCrystalUse(true);
}

// ##################################################################################################################################

int OBR_Gyro::getSensorAngle() {

  imu::Vector<3> euler = bno.getVector(Adafruit_BNO055::VECTOR_EULER);
  return (int)euler.x();
}

// ##################################################################################################################################

void OBR_Gyro::resetAngle() {

  _lastAngle = getSensorAngle();
}

// ##################################################################################################################################

int OBR_Gyro::getAngle(int targetAngle) {

  int finalAngle;

  if (targetAngle >= 0) {
    finalAngle = getSensorAngle() + (360-_lastAngle);
    finalAngle = finalAngle % 360;
  }


  else if (targetAngle < 0) {
    finalAngle = getSensorAngle() + (360-_lastAngle);
    finalAngle = finalAngle % 360;
    finalAngle = finalAngle - 360;
    finalAngle = finalAngle % 360;
  }

  return finalAngle;
}

