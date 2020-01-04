/*
  OBR_Robot.cpp - Logic control class definition.
  Created by Rafael Piacsek, May 24, 2017.
  Private code, for OBR use only.
*/


#include "Arduino.h"
#include "OBR_Robot.h"



OBR_Robot::OBR_Robot() {
  Serial.println("Robot initiated.");
}

// ##################################################################################################################################

void OBR_Robot::calibrate() {

    _QA.calibrateThreshold();
    _QA.calibrateOffset();
}

// ##################################################################################################################################

void OBR_Robot::run() {
  
  firstStage();
  secondStage(); 
  
}

// ##################################################################################################################################

void OBR_Robot::turn(byte direction) {

  _Gyro.resetAngle();

  if (direction == LEFT) {
    _Gyro.resetAngle();
    while(_Gyro.getAngle(LEFT) >= LEFT) {_Motor.move(-100, 100);}
    _Motor.move(_targetSpeed, _targetSpeed);
  }

  else if (direction == RIGHT) {
    _Gyro.resetAngle();
    while(_Gyro.getAngle(RIGHT) <= RIGHT) {_Motor.move(100, -100); Serial.println(RIGHT);}
    _Motor.move(_targetSpeed, _targetSpeed);
  }

  else if (direction == AROUND) {
    _Gyro.resetAngle();
    while(_Gyro.getAngle(AROUND) <= AROUND) {_Motor.move(100, -100);}
    _Motor.move(_targetSpeed, _targetSpeed);
  }
  
  
}

// ##################################################################################################################################

void OBR_Robot::firstStage() {
  
  while (true) {
    // Update QAs.
    _QA.updateAll();
    
    // Update Ultrasound' readings.
    avoidObstacle(RIGHT);

    // If more that 3 QAs are activated.
    if (_QA.blackCount > 2) {

      // Continue forward and update Reading.
      Serial.println("TEST");
      _Motor.move(100, 100);
      delay(100);
      _QA.updateAll();

      // If NORMAL CROSSING.
      if ((_QA.color[0] == _QA.BLACK) && (_QA.color[1] == _QA.BLACK) && (_QA.color[2] == _QA.BLACK) && (_QA.color[3] == _QA.BLACK) && (_QA.color[4] == _QA.BLACK) && (_QA.color[5] == _QA.BLACK)) {
        Serial.println("NORMAL CROSSING");
        _Motor.move(100, 100);
        delay(100);
      }

      // If TURN AROUND.
      else if ((_QA.color[0] == _QA.WHITE) && (_QA.color[1] == _QA.BLACK) && (_QA.color[2] == _QA.BLACK) && (_QA.color[3] == _QA.BLACK) && (_QA.color[4] == _QA.BLACK) && (_QA.color[5] == _QA.WHITE)) {
        _Motor.move(100, 100);
        delay(250);
        _QA.updateAll();
        if (_QA.blackCount == 6) {
          Serial.println("TURN AROUND");
          _Motor.move(100, 100);
          delay(350);
          turn(AROUND);
        }
        
      }

      // If GREEN LEFT.
      else if ((_QA.color[0] == _QA.WHITE) && (_QA.color[1] == _QA.BLACK) && (_QA.color[2] == _QA.BLACK) && (_QA.color[3] == _QA.BLACK) && (_QA.color[4] == _QA.WHITE) && (_QA.color[5] == _QA.WHITE)) {
        _Motor.move(100, 100);
        delay(250);
        _QA.updateAll();
        if ((_QA.color[0] == _QA.BLACK) && (_QA.color[1] == _QA.BLACK) && (_QA.color[2] == _QA.BLACK)) {
          Serial.println("GREEN LEFT");
          _Motor.move(100, 100);
          delay(350);
          turn(LEFT);
        }
      }

      // If GREEN RIGHT.
      else if ((_QA.color[0] == _QA.WHITE) && (_QA.color[1] == _QA.WHITE) && (_QA.color[2] == _QA.BLACK) && (_QA.color[3] == _QA.BLACK) && (_QA.color[4] == _QA.BLACK) && (_QA.color[5] == _QA.WHITE)) {
        _Motor.move(100, 100);
        delay(250);
        _QA.updateAll();
        if ((_QA.color[3] == _QA.BLACK) && (_QA.color[4] == _QA.BLACK) && (_QA.color[5] == _QA.BLACK)) {
          Serial.println("GREEN RIGHT");
          _Motor.move(100, 100);
          delay(350);
          turn(RIGHT);
        }
      }

      // If HARD LEFT.
      else if ((_QA.color[0] == _QA.BLACK) && (_QA.color[1] == _QA.BLACK) && (_QA.color[2] == _QA.BLACK) && (_QA.color[3] == _QA.BLACK) && (_QA.color[4] == _QA.WHITE) && (_QA.color[5] == _QA.WHITE)) {
        _Motor.move(100, 100);
        delay(250);
        _QA.updateAll();
        if (_QA.blackCount == 0) {
          Serial.println("HARD LEFT");
          _Motor.move(100, 100);
          delay(200);
          turn(LEFT);
        }
      }

      // If HARD RIGHT.
      else if ((_QA.color[0] == _QA.WHITE) && (_QA.color[1] == _QA.WHITE) && (_QA.color[2] == _QA.BLACK) && (_QA.color[3] == _QA.BLACK) &&(_QA.color[4] == _QA.BLACK) && (_QA.color[5] == _QA.BLACK)) {
        _Motor.move(100, 100);
        delay(250);
        _QA.updateAll();
        if (_QA.blackCount == 0) {
          Serial.println("HARD RIGHT");
          _Motor.move(100, 100);
          delay(200);
          turn(RIGHT);
        }
      }
    }

    // PID.
    else {
      Serial.print("PID - ");
      int PIDCorrection = _PID.compute(_QA.position);
      Serial.println(PIDCorrection);
      _Motor.move(_targetSpeed-PIDCorrection, _targetSpeed+PIDCorrection);

    }
  }
}

// ##################################################################################################################################

void OBR_Robot::secondStage() {

  // BALL RESCUE GOES HERE!!
  
}

// ##################################################################################################################################

void OBR_Robot::avoidObstacle(byte direction) {

  // Read distance in CM.
  int distance = _UltrasoundFront.ping_cm();

  // If distance is between 0 and 2.
  if ((distance <= 3) && (distance != 0) && (direction == RIGHT)) {
    turn(RIGHT);
    
    while (_UltrasoundLeft.ping_cm() != 0) {_Motor.move(100, 100);}
    _Motor.move(100, 100);
    delay(1000);

    turn(LEFT);
    _Motor.move(100, 100);
    delay(1000);
    while (_UltrasoundLeft.ping_cm() != 0) {_Motor.move(100, 100);}
    _Motor.move(100, 100);
    delay(1000);
  
    turn(LEFT);
    _QA.updateAll();
    while (_QA.blackCount != 6) {
      _QA.updateAll();
      _Motor.move(100, 100);
    }
    _Motor.move(100, 100);
    delay(500);

    turn(RIGHT);
    
    while (_UltrasoundBack.ping_cm() >= 3) {_Motor.move(-75, -75);}
    _Motor.move(_targetSpeed, _targetSpeed);
  }


  else if ((distance <= 2) && (distance != 0) && (direction == LEFT)) {
    turn(LEFT);
    
    while (_UltrasoundRight.ping_cm() != 0) {_Motor.move(100, 100);}
    _Motor.move(100, 100);
    delay(1000);

    turn(RIGHT);
    _Motor.move(100, 100);
    delay(1000);
    while (_UltrasoundRight.ping_cm() != 0) {_Motor.move(100, 100);}
    _Motor.move(100, 100);
    delay(100);
  
    turn(RIGHT);
    _QA.updateAll();
    while (_QA.blackCount != 6) {
      _QA.updateAll();
      _Motor.move(100, 100);
    }
    _Motor.move(100, 100);
    delay(500);

    turn(LEFT);

    while (_UltrasoundBack.ping_cm() >= 3) {_Motor.move(-75, -75);}
    _Motor.move(_targetSpeed, _targetSpeed);
  }
}


// ##################################################################################################################################

void OBR_Robot::debug(int delayTime) {
  
  _QA.updateAll();
  
//  Serial.print("0: ");
  Serial.print(_QA.rawValue[0]);
  Serial.print("  | ");

//  Serial.print("1: ");
  Serial.print(_QA.rawValue[1]);
  Serial.print("  | ");

//  Serial.print("2: ");
  Serial.print(_QA.rawValue[2]);
  Serial.print("  | ");

//  Serial.print("3: ");
  Serial.print(_QA.rawValue[3]);
  Serial.print("  | ");

//  Serial.print("4: ");
  Serial.print(_QA.rawValue[4]);
  Serial.print("  | ");

//  Serial.print("5: ");
  Serial.print(_QA.rawValue[5]);
  Serial.print("  | ");

  Serial.print("Position: ");
  Serial.print(_QA.position);
  Serial.print("  | ");

  Serial.println();

//  //  Serial.print("0: ");
//  Serial.print(_QA.color[0]);
//  Serial.print("    | ");
//
////  Serial.print("1: ");
//  Serial.print(_QA.color[1]);
//  Serial.print("    | ");
//
////  Serial.print("2: ");
//  Serial.print(_QA.color[2]);
//  Serial.print("    | ");
//
////  Serial.print("3: ");
//  Serial.print(_QA.color[3]);
//  Serial.print("    | ");
//
////  Serial.print("4: ");
//  Serial.print(_QA.color[4]);
//  Serial.print("    | ");
//
////  Serial.print("5: ");
//  Serial.print(_QA.color[5]);
//  Serial.print("    | ");
//
//  Serial.print("Position: ");
//  Serial.print(_QA.blackCount);
//  Serial.print("    | ");
//  
//  Serial.println();
//  Serial.println();


  _QA.calibrateThreshold(100);
  Serial.println();

  delay(delayTime);
    
}

