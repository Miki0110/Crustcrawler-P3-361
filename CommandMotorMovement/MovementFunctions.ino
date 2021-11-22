//MOTOR MAX/MIN Values (UNIT_RAW)
int MotorMinAngleRaw[] = {1800,//Motor1
                          750,//Motor2
                          740,//Motor3
                          750,//Motor4
                          700
                         };//Motor5
int MotorMaxAngleRaw[] = {1800, 2500, //Motor1
                          3320,//Motor2
                          3320,//Motor3
                          2450,//Motor4
                          2450
                         };//Motor5
int PlateCoordinateMin[] = {0, -20, 0}; //IN CENTIMETERS
int PlateCoordinateMax[] = {38, 36, 20}; //IN CENTIMETERS

//int Plate0point[]{1828,,}; //RAW values



void CheckMovementAngleRAW(int x, int y, int z) {
  int newpos[] = {x, y, z};
  for (int i = 1; i <= 3; i++) {
    if (newpos[i - 1] >= PlateCoordinateMin[i - 1] && newpos[i - 1] <= PlateCoordinateMax[i - 1]) { //If the position is between the limits:
    }
    else {
    }
  }
}
void CheckMovementCoordinate(int x, int y, int z) {
  int newpos[] = {x, y, z};
  for (int i = 1; i <= 3; i++) {
    if (newpos[i - 1] >= MotorMinAngleRaw[i - 1] && newpos[i - 1] <= MotorMaxAngleRaw[i - 1]) { //If the position is between the limits:
    }
    else {
    }
  }
}



void GoTo2D(int xCoordinate, int yCoordinate) { //GoTo for 2 axis(Normal when playing chess and move around the board)

  BLA::Matrix<1, 3> Posvinkel = setCartesianPosition((double) xCoordinate, (double) yCoordinate, 0);

  dxl.setGoalPosition(DXL_ID[1], Posvinkel(0, 0) * (180 / PI), UNIT_DEGREE);
  dxl.setGoalPosition(DXL_ID[2], Posvinkel(0, 1) * (180 / PI), UNIT_DEGREE);
  dxl.setGoalPosition(DXL_ID[3], Posvinkel(0, 2) * (180 / PI), UNIT_DEGREE);

  Serial.print(theta1);
  Serial.print(" , ");
  Serial.print(theta2);
  Serial.print(" , ");
  Serial.println(theta3);
  while (dxl.getPresentPosition(DXL_ID[1], UNIT_DEGREE) > ((Posvinkel(0, 0) * (180 / PI)) + 5) || dxl.getPresentPosition(DXL_ID[1], UNIT_DEGREE) < ((Posvinkel(0, 0) * (180 / PI)) - 5)) {Serial.println("Moving Theta1");}; //As long as not near pos, move the robot
  while (dxl.getPresentPosition(DXL_ID[2], UNIT_DEGREE) > ((Posvinkel(0, 1) * (180 / PI)) + 5) || dxl.getPresentPosition(DXL_ID[2], UNIT_DEGREE) < ((Posvinkel(0, 1) * (180 / PI)) - 5)) {Serial.println("Moving Theta2");}; //As long as not near pos, move the robot
  while (dxl.getPresentPosition(DXL_ID[3], UNIT_DEGREE) > ((Posvinkel(0, 2) * (180 / PI)) + 5) || dxl.getPresentPosition(DXL_ID[3], UNIT_DEGREE) < ((Posvinkel(0, 2) * (180 / PI)) - 5)) {Serial.println("Moving Theta3");}; //As long as not near pos, move the robot
};

void GoTo3D(int xCoordinate, int yCoordinate, int zCoordinate) { //GoTo for 3 axis(Z included)

  setCartesianPosition((double) xCoordinate, (double) yCoordinate, (double) zCoordinate);


  //dxl.setGoalPosition(DXL_ID[1], theta1, UNIT_RAW);
  //dxl.setGoalPosition(DXL_ID[2], theta2, UNIT_RAW);
  //dxl.setGoalPosition(DXL_ID[3], theta3, UNIT_RAW);

  Serial.print(theta1);
  Serial.print(" , ");
  Serial.print(theta2);
  Serial.print(" , ");
  Serial.println(theta3);

}
