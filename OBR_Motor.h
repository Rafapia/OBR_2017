/*
  OBR_Motor.h - Motor control class header.
  Created by Rafael Piacsek, May 21, 2017.
  Private code, for OBR use only.
*/


#ifndef OBR_Motor_h
#define OBR_Motor_h

#include "Arduino.h"

// Declare class Motor. 
class OBR_Motor {
  
  private:
  
    // Left motor pins.
    byte _leftPin_1;
    byte _leftPin_2;
    byte _leftPinPwm;
    
    // Right motor pins.
    byte _rightPin_1;
    byte _rightPin_2;
    byte _rightPinPwm;
    
  public:
  
    // Status and control variables.
    byte _leftSpeed;
    byte _rightSpeed;
   
    // Declare some constants.
    #define LEFT_MOTOR true
    #define RIGHT_MOTOR false
    #define FORWARD true
    #define BACKWARD false
    
    // Constructor method.
    OBR_Motor(byte leftPin_1, byte leftPin_2, byte leftPinPwm, byte rightPin_1, byte rightPin_2, byte rightPinPwm);
    
    // Methods.
    void move(int leftSpeed=0, int rightSpeed=0);
    void setDirection(bool motor, bool motorDirection);
    void stop();

};

#endif
