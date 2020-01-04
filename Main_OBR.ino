 /*
  Main_Program.ino - Main tab for robot control.
  Created by Rafael Piacsek, May 22, 2017.
  Private code, for OBR use only.
*/

#include "OBR_Robot.h"
//#include "OBR_Motor.h" 


//OBR_Motor _Motor = OBR_Motor(17, 16, 2, 15, 14, 3);


void setup(){

  Serial.begin(9600);
  delay(50);

  OBR_Robot Robot = OBR_Robot();
  Serial.println("Setup done!");

//  Robot.run();
//  Robot.debug(750);
  Robot.turn(Robot.RIGHT);
}

void loop(){
  
}
