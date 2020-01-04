/*
  OBR_Robot.h - Logic control class header.
  Created by Rafael Piacsek, May 24, 2017.
  Private code, for OBR use only.
*/


#ifndef OBR_Robot_h
#define OBR_Robot_h


#include "Arduino.h"
#include "OBR_Motor.h"
#include "OBR_QA.h"
#include "OBR_PID.h"
#include "OBR_Gyro.h"
#include <NewPing.h>



/* Classes parameters:

// OBR_Motor.h
(byte left_1,   byte left_2,   byte leftPwm,   byte right_1,   byte right_2,   byte rightPwm)

// OBR_QA.h
(byte QAPin_0, byte QAPin_1, byte QAPin_2, byte QAPin_3)

// OBR_Ultrasound.h
(byte trigPin, byte echoPin)

// OBR_PID.h
(float kP, float kI, float kD)
*/



class OBR_Robot {
  
  private:
    // Instantiate all necessary objects.
    OBR_Motor _Motor = OBR_Motor(17, 16, 2, 15, 14, 3); 
    OBR_QA _QA = OBR_QA(A10, A11, A12, A13, A14, A15);
    NewPing _UltrasoundFront = NewPing(53, 52, 15);  // (trigPin, echoPin, maxDist)
    NewPing _UltrasoundBack = NewPing(49, 48, 15);
    NewPing _UltrasoundLeft = NewPing(47, 46, 15);
    NewPing _UltrasoundRight = NewPing(51, 50, 15);

    // PID-related variables and objects.
    OBR_PID _PID = OBR_PID(.1, .001, 0.01);

    // Gyro-related object.
    OBR_Gyro _Gyro = OBR_Gyro();
    
    // Target speed.
    const int _targetSpeed = 200; // DEFAULT IS 200!


  public:
  
    // Define Contructor method.
    OBR_Robot();

    // Define some values.
    byte LEFT = -90;
    byte RIGHT = 90;
    byte AROUND = 180;
    
    // Methods.
    void debug(int delayTime=500);
    void calibrate();
    void run();
    void firstStage();
    void secondStage();
    void avoidObstacle(byte direction);
    void turn(byte direction);
    
  
};


#endif

