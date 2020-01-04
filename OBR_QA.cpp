/*
  OBR_QA.cpp - QA control class definition.
  Created by Rafael Piacsek, May 21, 2017.
  Private code, for OBR use only.
*/


#include "Arduino.h"
#include "OBR_QA.h"

// ##################################################################################################################################

// Define Contructor method.
OBR_QA::OBR_QA(byte QAPin_0, byte QAPin_1, byte QAPin_2, byte QAPin_3, byte QAPin_4, byte QAPin_5) {
  /*
  Inputs:  QAPin_0  (byte - QA_0 pin)
           QAPin_1  (byte - QA_1 pin)
  Returns: NONE
  
  Description:
    Contructor method. It is called whenever this class is instantiated, and sets all
  necessary variables to operate QAs.
  */
  
  _QAPin[0] = QAPin_0;
  _QAPin[1] = QAPin_1;
  _QAPin[2] = QAPin_2;
  _QAPin[3] = QAPin_3;
  _QAPin[4] = QAPin_4;
  _QAPin[5] = QAPin_5;
  
  // Call update methods once to initiate variables, avoiding accessing uninitialized variables.
  updateValueMean();
  updateColorMean();
  calibrate();
  Serial.println("QAs initiated.");
}

// ##################################################################################################################################

// Define updateAll method
void OBR_QA::updateValueMean(byte n) {
  /*
  Inputs: n (optional byte - number of iterations to average)
  Returns: NONE
  
  Description:
    This method is responsible for updating all readings from QA sensors based on the mean of n readings.
  */
  
  rawValue[0] = 0;
  rawValue[1] = 0;
  rawValue[2] = 0;
  rawValue[3] = 0;
  rawValue[4] = 0;
  rawValue[5] = 0;
  
  // Ensure n!=0 to avoid errors on division.
  if (n!=0) {
  
    for (byte i=0; i<n; i++) {
      rawValue[0] += analogRead(_QAPin[0]);
      rawValue[1] += analogRead(_QAPin[1]);
      rawValue[2] += analogRead(_QAPin[2]);
      rawValue[3] += analogRead(_QAPin[3]);
      rawValue[4] += analogRead(_QAPin[4]);
      rawValue[5] += analogRead(_QAPin[5]);
    }
    
    rawValue[0] /= n;
    rawValue[1] /= n;
    rawValue[2] /= n;
    rawValue[3] /= n;
    rawValue[4] /= n;
    rawValue[5] /= n;
  
  }
}

// ##################################################################################################################################

// Define updateAll method
void OBR_QA::updateColorMean(byte n) {
  /*
  Inputs: NONE
  Returns: NONE
  
  Description:
    This method is responsible for updating all colors from QA sensors.
  */
    
  blackCount = 0;
  for (byte i = 0; i < 6; i++) {
    if (rawValue[i] >= _QAThresh[i]) {
      blackCount++;
      color[i] = BLACK;
    } else if (rawValue[i] < _QAThresh[i]) {
      color[i] = WHITE;
    }
  }
}

// ##################################################################################################################################

void OBR_QA::updatePosition() {
 
 position = (rawValue[2] - _QADefault[2]) + 6*(rawValue[1] - _QADefault[1]) + 18*(rawValue[0] - _QADefault[0]) 
          - (rawValue[3] - _QADefault[3]) - 6*(rawValue[4] - _QADefault[4]) - 18*(rawValue[5] - _QADefault[5]);
}

// ##################################################################################################################################

void OBR_QA::updateAll() {
  
  updateValueMean();
  updateColorMean();
  updatePosition();
}

// ##################################################################################################################################

void OBR_QA::calibrate(byte n) {

  // DEFAULT READING CALIBRATION FOR PID.
  for (int i = 0; i < n; i++) {
      // Update Reading.
      updateValueMean();
  
      // Add readings to array.
      _QADefault[0] += rawValue[0];
      _QADefault[1] += rawValue[1];
      _QADefault[2] += rawValue[2];
      _QADefault[3] += rawValue[3];
      _QADefault[4] += rawValue[4];
      _QADefault[5] += rawValue[5];

      delay(50);
  }

  // Average all readings.
  _QADefault[0] /= n;
  _QADefault[1] /= n;
  _QADefault[2] /= n;
  _QADefault[3] /= n;
  _QADefault[4] /= n;
  _QADefault[5] /= n;


  // THRESHOLD CALIBRATION FOR LOGIC.
  // Reset Threshld array.
  _QAThresh[0] = 0;
  _QAThresh[1] = 0;
  _QAThresh[2] = 0;
  _QAThresh[3] = 0;
  _QAThresh[4] = 0;
  _QAThresh[5] = 0;

  // Get largest reading from n readings.
  for (int i = 0; i < n; i++) {
    // Update Reading.
    updateValueMean();

    // Get largest reading.
    if (rawValue[0] > _QAThresh[0]) {_QAThresh[0] = rawValue[0];}
    if (rawValue[1] > _QAThresh[1]) {_QAThresh[1] = rawValue[1];}
    if (rawValue[2] > _QAThresh[2]) {_QAThresh[2] = rawValue[1];}
    if (rawValue[3] > _QAThresh[3]) {_QAThresh[3] = rawValue[4];}
    if (rawValue[4] > _QAThresh[4]) {_QAThresh[4] = rawValue[4];}
    if (rawValue[5] > _QAThresh[5]) {_QAThresh[5] = rawValue[5];}
  }

  // Threshold should be 160% of largest reading for each sensor.
  _QAThresh[0] = _QAThresh[0]*1.5;
  _QAThresh[1] = _QAThresh[1]*1.5;
  _QAThresh[2] = _QAThresh[2]*1.5;
  _QAThresh[3] = _QAThresh[3]*1.5;
  _QAThresh[4] = _QAThresh[4]*1.5;
  _QAThresh[5] = _QAThresh[5]*1.5;
}


// ##################################################################################################################################

void OBR_QA::calibrateOffset(byte n) {

    for (int i = 0; i < n; i++) {
      // Update Reading.
      updateValueMean();
  
      // Add readings to array.
      _QADefault[0] += rawValue[0];
      _QADefault[1] += rawValue[1];
      _QADefault[2] += rawValue[2];
      _QADefault[3] += rawValue[3];
      _QADefault[4] += rawValue[4];
      _QADefault[5] += rawValue[5];

      delay(50);
  }

  // Average all readings.
  _QADefault[0] /= n;
  _QADefault[1] /= n;
  _QADefault[2] /= n;
  _QADefault[3] /= n;
  _QADefault[4] /= n;
  _QADefault[5] /= n;
  
}

// ##################################################################################################################################

void OBR_QA::calibrateThreshold(byte n) {

  // Reset Threshld array.
  _QAThresh[0] = 0;
  _QAThresh[1] = 0;
  _QAThresh[2] = 0;
  _QAThresh[3] = 0;
  _QAThresh[4] = 0;
  _QAThresh[5] = 0;


  for (int i = 0; i < n; i++) {
    // Update Reading.
    updateValueMean();

    // Get largest reading.
    if (rawValue[0] > _QAThresh[0]) {_QAThresh[0] = rawValue[0];}
    if (rawValue[1] > _QAThresh[1]) {_QAThresh[1] = rawValue[1];}
    if (rawValue[2] > _QAThresh[2]) {_QAThresh[2] = rawValue[2];}
    if (rawValue[3] > _QAThresh[3]) {_QAThresh[3] = rawValue[3];}
    if (rawValue[4] > _QAThresh[4]) {_QAThresh[4] = rawValue[4];}
    if (rawValue[5] > _QAThresh[5]) {_QAThresh[5] = rawValue[5];}
  }

  // Threshold should be +30% of largest reading for each sensor.
  _QAThresh[0] = _QAThresh[0]*1.5;
  _QAThresh[1] = _QAThresh[1]*1.5;
  _QAThresh[2] = _QAThresh[2]*1.5;
  _QAThresh[3] = _QAThresh[3]*1.5;
  _QAThresh[4] = _QAThresh[4]*1.5;
  _QAThresh[5] = _QAThresh[5]*1.5;

  Serial.print(_QAThresh[0]);
  Serial.print("  |   ");
  Serial.print(_QAThresh[1]);
  Serial.print("  |   ");
  Serial.print(_QAThresh[2]);
  Serial.print("  |   ");
  Serial.print(_QAThresh[3]);
  Serial.print("  |   ");
  Serial.print(_QAThresh[4]);
  Serial.print("  |   ");
  Serial.print(_QAThresh[5]);
  Serial.println("  |   ");
  
}
