/*
  OBR_Motor.cpp - Motor control class definition.
  Created by Rafael Piacsek, May 21, 2017.
  Private code, for OBR use only.
*/

#include "Arduino.h"
#include "OBR_Motor.h"

// ##################################################################################################################################

// Define contructor method.
OBR_Motor::OBR_Motor(byte leftPin_1, byte leftPin_2, byte leftPinPwm, byte rightPin_1, byte rightPin_2, byte rightPinPwm) {
  /*
  Inputs: left_1  (byte - correspondant pin for Left Port 1)
          left_2  (byte - correspondant pin for Left Port 2)
          leftPWM  (byte - correspondant pin for Left Port PWM)
          right_1  (byte - correspondant pin for Right Port 1)
          right_2  (byte - correspondant pin for Right Port 2)
          rightPwm  (byte - correspondant pin for Right Port PWM)
  Returns: NONE
  
  Description:
    Contructor method. It is called whenever this class is instantiated, and sets all
  necessary variables to operate motors. 
  */
  
  // Set private pin variables.
  _leftPin_1 = leftPin_1;
  _leftPin_2 = leftPin_2;
  _leftPinPwm = leftPinPwm;
  _rightPin_1 = rightPin_1;
  _rightPin_2 = rightPin_2;
  _rightPinPwm = rightPinPwm;
  
  
  // Set pinMode for those pins.
  pinMode(_leftPin_1, OUTPUT);
  pinMode(_leftPin_2, OUTPUT);
  pinMode(_leftPinPwm, OUTPUT);
  pinMode(_rightPin_1, OUTPUT);
  pinMode(_rightPin_2, OUTPUT);
  pinMode(_rightPinPwm, OUTPUT);

  Serial.println("Motors initiated.");
}

// ##################################################################################################################################

// Define setDirection method.
void OBR_Motor::setDirection(bool motor, bool motorDirection) {
  /*
  Inputs: motor (bool - desired motor to perform operation)
          motorDirection (bool - desired direction to set motor)
  Returns: NONE
  
  Descriprion:
    Given that one can't pass a negative PWM value, this methos is responsible for
  checking if the value passed is a positive natural number within bounds (-255 <= n <= 255),
  and based on the input number, set PWMs (invert direction if negative).
  */
  
  // Check which motor to set direction.
  switch (motor) {
    
    // In case the motor passed is Left...
    case (LEFT_MOTOR):
      // Check which direction to set Left motor.
      switch (motorDirection) {
        // In case the direction passed if FORWARD...
        case (FORWARD):
          digitalWrite(_leftPin_1, LOW);
          digitalWrite(_leftPin_2, HIGH);
          break;
        // In case the direction passed if BACKWARD...
        case (BACKWARD):
          digitalWrite(_leftPin_1, HIGH);
          digitalWrite(_leftPin_2, LOW);
          break;
      }
      break;
      
      
    case (RIGHT_MOTOR):
      
      // Check which direction to set Right motor.
      switch (motorDirection) {
        // In case the direction passed if FORWARD...
        case (FORWARD):
          digitalWrite(_rightPin_1, LOW);
          digitalWrite(_rightPin_2, HIGH);
          break;
        // In case the direction passed if BACKWARD...
        case (BACKWARD):
          digitalWrite(_rightPin_1, HIGH);
          digitalWrite(_rightPin_2, LOW);
          break;
      }
      break; 
  }
}
    
// ##################################################################################################################################    
    
// Define move method.    
void OBR_Motor::move(int leftSpeed, int rightSpeed) {
  /*
  Inputs: leftSpeed (optional int - desired speed for Left motor)
          rightSpeed (optional int - desired speed for Right motor)
  Returns: NONE
  
  Descritpion:
    This method is responsible for, given a speed value from -255 to 255, applying that speed to
  the correspondant motor.
  */
  
  // Check if parameters are within limits
  leftSpeed = constrain(leftSpeed, -255, 255);
  rightSpeed = constrain(rightSpeed, -255, 255);
    
  // Update speed variables.
  _rightSpeed = rightSpeed;
  _leftSpeed = leftSpeed;
  
  // Set Left motor direction.
  if (leftSpeed > 0) {
    setDirection(LEFT_MOTOR, FORWARD);
  } else if (leftSpeed < 0) {
    setDirection(LEFT_MOTOR, BACKWARD);
  } else if(leftSpeed == 0) {
    digitalWrite(_leftPin_1, HIGH);
    digitalWrite(_leftPin_2, HIGH);
  }

  // Set Right motor direction.
  if (rightSpeed > 0) {
    setDirection(RIGHT_MOTOR, FORWARD);
  } else if (rightSpeed < 0) {
    setDirection(RIGHT_MOTOR, BACKWARD);
  } else if (rightSpeed == 0) {
    digitalWrite(_rightPin_1, HIGH);
    digitalWrite(_rightPin_2, HIGH);
  }

  // Set PWM values.
  analogWrite(_leftPinPwm, abs(leftSpeed));
  analogWrite(_rightPinPwm, abs(rightSpeed));
    

}

// ##################################################################################################################################

// Define stop method.
void OBR_Motor::stop() {
  /*
  Inputs: NONE
  Returns: NONE
  
  Descritpion:
    This method is responsible for reseting the motor's pwm values and
  setting both ports of each motor to HIGH, which ensures that the robot
  will come to a full stop.
  */
  
  // Update speed variables.
  _leftSpeed = 0;
  _rightSpeed = 0;
  
  // Stop Left motor.
  digitalWrite(_leftPin_1, HIGH);
  digitalWrite(_leftPin_2, HIGH);
  analogWrite(_leftPinPwm, 0);
  
  // Stop Left motor.
  digitalWrite(_rightPin_1, HIGH);
  digitalWrite(_rightPin_2, HIGH);
  analogWrite(_rightPinPwm, 0);
  
}

// ##################################################################################################################################
