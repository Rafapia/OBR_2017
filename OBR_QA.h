/*
  OBR_QA.h - QA control class header.
  Created by Rafael Piacsek, May 21, 2017.
  Private code, for OBR use only.
*/


#ifndef OBR_QA_h
#define OBR_QA_h

#include "Arduino.h"


#define QA_0 0
#define QA_1 1
#define QA_2 2
#define QA_3 3
#define QA_4 4
#define QA_5 5


// Declare class QA. 
class OBR_QA {
  
  private:
    
    // OBR_QA pins.
    byte _QAPin[6];
    
    // QAs calibration value.
    int _QAThresh[6] = {643, 183, 180, 300, 316, 486};  // {181, 348, 161, 284, 98, 54}, {72, 348, 161, 284, 98, 54}
    int _QADefault[6] = {409, 105, 448, 617, 213, 323};
    
  public:    
    
    // QA status.
    int rawValue[6];
    byte color[6];
    byte blackCount = 0;
    int position = 0;
    
    // Define colors.
    const byte BLACK = 1;
    const byte WHITE = 0;
    
    // Contructor method.
    OBR_QA(byte QAPin_0, byte QAPin_1, byte QAPin_2, byte QAPin_3, byte QAPin_4, byte QAPin_5);
    
    // Methods.
    void calibrateOffset(byte n=10);
    void calibrateThreshold(byte n=10);
    void calibrate(byte n=10);
    void updateValueMean(byte n=10);
    void updateColorMean(byte n=10);
    void updatePosition();
    void updateAll();
    
    
  
};
  
#endif

