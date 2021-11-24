//Cannot be applied in current control mode
boolean setSpeedLimit(float speedPct, double unit, uint8_t id) {
  int VELOCITY_LIMIT = 1023;
  double VELOCITY_UNIT = unit; // rpm
  double maxSpeed = VELOCITY_LIMIT * VELOCITY_UNIT;
  uint32_t newSpeed = speedPct * maxSpeed; // rpm
  uint32_t writeTimeout = 100; // ms
  dxl.writeControlTableItem(PROFILE_VELOCITY, id, newSpeed, writeTimeout);
}

//Cannot be applied in current control mode
boolean setAccelerationLimit(float speedPCT, double unit, uint8_t id) {
  int VELOCITY_LIMIT = 1023;
  double VELOCITY_UNIT = unit; // rpm
  double maxSpeed = VELOCITY_LIMIT * VELOCITY_UNIT;
  uint32_t newSpeed = speedPCT * maxSpeed; // rpm
  uint32_t writeTimeout = 100; // ms
  dxl.writeControlTableItem(PROFILE_VELOCITY, id, newSpeed, writeTimeout);
}

boolean setCurrentLimit(uint32_t currentLimit, uint8_t id) {
  dxl.ping(id);
  dxl.torqueOff(id);
  dxl.writeControlTableItem(CURRENT_LIMIT, id, currentLimit);
  dxl.torqueOn(id);
}






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
