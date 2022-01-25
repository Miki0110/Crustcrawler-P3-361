
float AngleDiff = 5; //The unsernaty for the motor angle pos for 1 side(-/+)

BLA::Matrix<1, 3> MoveablePos = {0, 0, 0};

void Move(int xChange, int yChange, int zChange) {//________________________________________//________________________________________
  int NextPos[] = {xChange, yChange, zChange};
  BLA::Matrix<4, 4> OldPos = GetCurrentPos();

  GoTo(OldPos(0, 3) + NextPos[0], OldPos(1, 3) + NextPos[1], OldPos(2, 3) + NextPos[2]);
};//End of Move()



void GoTo(double xCoordinate, double yCoordinate, double zCoordinate) { //GoTo for 3 axis(Z included)//________________________________________//________________________________________
  MoveablePos = {xCoordinate, yCoordinate, zCoordinate};
  //BLA::Matrix<1, 3> MoveablePos = CheckMovementCoordinate(xCoordinate, yCoordinate, zCoordinate); //Checks if the Coordinate positions are reachable
  //CheckMovementAngleDEGREE

  BLA::Matrix<1, 3> Posvinkel = setCartesianPosition((double) MoveablePos(0, 0), (double) MoveablePos(0, 1), (double) MoveablePos(0, 2));
  BLA::Matrix<1, 3> MoveablePosvinkel = setCartesianPosition((double) MoveablePos(0, 0), (double) MoveablePos(0, 1), (double) MoveablePos(0, 2));
  //BLA::Matrix<1, 3> MoveablePosvinkel = CheckMovementAngleDEGREE(Posvinkel(0,0),Posvinkel(0,1),Posvinkel(0,2)); //Checks if the positions are reachable


  dxl.setGoalPosition(DXL_ID[1], MoveablePosvinkel(0, 0) , UNIT_DEGREE);
  dxl.setGoalPosition(DXL_ID[2], MoveablePosvinkel(0, 1), UNIT_DEGREE);
  dxl.setGoalPosition(DXL_ID[3], MoveablePosvinkel(0, 2) , UNIT_DEGREE);

  //These next lines waits for the robot to get inside an area of (AngleDiff) of the end pos, before moving to the next pos
  //while (dxl.getPresentPosition(DXL_ID[1], UNIT_DEGREE) > ((MoveablePosvinkel(0, 0)) + AngleDiff) || dxl.getPresentPosition(DXL_ID[1], UNIT_DEGREE) < ((MoveablePosvinkel(0, 0)) - AngleDiff)) {Serial.println("Moving Theta1");}; //As long as not near pos, move the robot
  //while (dxl.getPresentPosition(DXL_ID[2], UNIT_DEGREE) > ((MoveablePosvinkel(0, 1)) + AngleDiff) || dxl.getPresentPosition(DXL_ID[2], UNIT_DEGREE) < ((MoveablePosvinkel(0, 1)) - AngleDiff)) {Serial.println("Moving Theta2");}; //As long as not near pos, move the robot
  //while (dxl.getPresentPosition(DXL_ID[3], UNIT_DEGREE) > ((MoveablePosvinkel(0, 2)) + AngleDiff) || dxl.getPresentPosition(DXL_ID[3], UNIT_DEGREE) < ((MoveablePosvinkel(0, 2)) - AngleDiff)) {Serial.println("Moving Theta3");}; //As long as not near pos, move the robot
  //LeanedAngle = (190 - dxl.getPresentPosition(DXL_ID[1], UNIT_DEGREE)) / 8; //Used to give the Wiggle/Swap for the endEffector
  updateGripper(); //Makes the  Wiggle/Swap of the end effector
};//End of GoTo()

void setPWM(uint8_t id, int16_t PWM){ //PWM control function
 
  if(PWM > PWMlimit){ //A check so we don't set too high values
    PWM=PWMlimit;
    }else if(PWM < -PWMlimit){
      PWM=-PWMlimit;
      }
  
  dxl.writeControlTableItem(GOAL_PWM, id, PWM); //send values to the motor
}
