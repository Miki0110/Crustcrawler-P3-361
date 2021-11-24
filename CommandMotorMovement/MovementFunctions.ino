//MOTOR MAX/MIN Values (UNIT_DEGREE)
int MotorMinAngleDEGREE[] = {158,//Motor1
                             65, //Motor2
                             65, //Motor3
                             65, //Motor4
                             61  //Motor5
                            };
int MotorMaxAngleDEGREE[] = {220,//Motor1
                             291,//Motor2
                             291,//Motor3
                             215,//Motor4
                             215 //Motor5
                            };
int PlateCoordinateMin[] = {00, -80, 00}; //IN MILLIMETERS
int PlateCoordinateMax[] = {350, 360, 200}; //IN MILLIMETERS
float AngleDiff = 0.5; //The unsernaty for the motor angle pos for 1 side(-/+)

BLA::Matrix<1, 3> MoveablePos = {0, 0, 0};



BLA::Matrix<1, 3> CheckMovementAngleDEGREE(int Theta1, int Theta2, int Theta3) {
  BLA::Matrix<1, 3> newpos = {Theta1, Theta2, Theta3};
  for (int i = 1; i <= 3; i++) {
    if (newpos(0, i - 1) <= MotorMinAngleDEGREE[i - 1]) { //If the position is under minimum:
      newpos(0, i - 1) = MotorMinAngleDEGREE[i - 1];
    }
    else if (newpos(0, i - 1) >= MotorMaxAngleDEGREE[i - 1]) { //If the position is over maximum:
      newpos(0, i - 1) = MotorMaxAngleDEGREE[i - 1];
    }
  }
  return newpos;
}

BLA::Matrix<1, 3> CheckMovementCoordinate(int x, int y, int z) {
  BLA::Matrix<1, 3> newpos = {x, y, z};
  for (int i = 1; i <= 3; i++) {
    if (newpos(0, i - 1) <= PlateCoordinateMin[i - 1]) { //If the position is under minimum:
      newpos(0, i - 1) = PlateCoordinateMin[i - 1];
    }
    else if (newpos(0, i - 1) >= PlateCoordinateMax[i - 1]) { //If the position is over maximum:
      newpos(0, i - 1) = PlateCoordinateMax[i - 1];
    }
  }
  return newpos;
}


void Move(int xChange, int yChange, int zChange) {
  int NextPos[] = {xChange, yChange, zChange};
  BLA::Matrix<4, 4> OldPos = GetCurrentPos();

  GoTo3D(OldPos(0, 3) + NextPos[0], OldPos(1, 3) + NextPos[1], OldPos(2, 3) + NextPos[2]);
};



void GoTo3D(int xCoordinate, int yCoordinate, int zCoordinate) { //GoTo for 3 axis(Z included)
  MoveablePos = {xCoordinate, yCoordinate, zCoordinate};
  //BLA::Matrix<1, 3> MoveablePos = CheckMovementCoordinate(xCoordinate, yCoordinate, zCoordinate); //Checks if the Coordinate positions are reachable

  //setCartesianPosition((double) xCoordinate, (double) yCoordinate, (double) zCoordinate);
  //CheckMovementAngleDEGREE

  BLA::Matrix<1, 3> Posvinkel = setCartesianPosition((double) MoveablePos(0, 0), (double) MoveablePos(0, 1), (double) MoveablePos(0, 2));
  BLA::Matrix<1, 3> MoveablePosvinkel = setCartesianPosition((double) MoveablePos(0, 0), (double) MoveablePos(0, 1), (double) MoveablePos(0, 2));
  //BLA::Matrix<1, 3> MoveablePosvinkel = CheckMovementAngleDEGREE(Posvinkel(0,0),Posvinkel(0,1),Posvinkel(0,2)); //Checks if the positions are reachable


  dxl.setGoalPosition(DXL_ID[1], MoveablePosvinkel(0, 0) , UNIT_DEGREE);
  dxl.setGoalPosition(DXL_ID[2], MoveablePosvinkel(0, 1), UNIT_DEGREE);
  dxl.setGoalPosition(DXL_ID[3], MoveablePosvinkel(0, 2) , UNIT_DEGREE);

  Serial.print(theta1);
  Serial.print(" , ");
  Serial.print(theta2);
  Serial.print(" , ");
  Serial.println(theta3);
  //while (dxl.getPresentPosition(DXL_ID[1], UNIT_DEGREE) > ((MoveablePosvinkel(0, 0)) + AngleDiff) || dxl.getPresentPosition(DXL_ID[1], UNIT_DEGREE) < ((MoveablePosvinkel(0, 0)) - AngleDiff)) {Serial.println("Moving Theta1");}; //As long as not near pos, move the robot
  //while (dxl.getPresentPosition(DXL_ID[2], UNIT_DEGREE) > ((MoveablePosvinkel(0, 1)) + AngleDiff) || dxl.getPresentPosition(DXL_ID[2], UNIT_DEGREE) < ((MoveablePosvinkel(0, 1)) - AngleDiff)) {Serial.println("Moving Theta2");}; //As long as not near pos, move the robot
  //while (dxl.getPresentPosition(DXL_ID[3], UNIT_DEGREE) > ((MoveablePosvinkel(0, 2)) + AngleDiff) || dxl.getPresentPosition(DXL_ID[3], UNIT_DEGREE) < ((MoveablePosvinkel(0, 2)) - AngleDiff)) {Serial.println("Moving Theta3");}; //As long as not near pos, move the robot
};
