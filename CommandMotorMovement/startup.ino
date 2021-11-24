//Control only through torque (recommended for use with dynamics)
//MX-106 and MX-64
boolean startupCurrent(uint8_t id) {
  dxl.ping(id);
  dxl.torqueOff(id);
  dxl.writeControlTableItem(OPERATING_MODE, id, 0);
  dxl.torqueOn(id);
}

//MX-106, MX-64 and MX-28
boolean startupVelocity(uint8_t id) {
  dxl.ping(id);
  dxl.torqueOff(id);
  dxl.writeControlTableItem(OPERATING_MODE, id, 1);
  dxl.torqueOn(id);
}

//MX-106, MX-64 and MX-28
boolean startupPosition(uint8_t id) {
  dxl.ping(id);
  dxl.torqueOff(id);
  dxl.writeControlTableItem(OPERATING_MODE, id, 3);
  dxl.torqueOn(id);
}

//Control through position, with torque limits available
//(Allows for position control with more compliance as links cam be moved if more torque than expected is required)
//MX-106 and MX-64
boolean startupCurrentPosition(uint8_t id) {
  dxl.ping(id);
  dxl.torqueOff(id);
  dxl.writeControlTableItem(OPERATING_MODE, id, 5);
  dxl.torqueOn(id);
}


void GoToStartPos() {
  const int Startpos[] = {190, 180, 270}; //Startpos for the motors (DEGREE)

  dxl.setGoalPosition(DXL_ID[1], Startpos[0], UNIT_DEGREE);
  dxl.setGoalPosition(DXL_ID[2], Startpos[1], UNIT_DEGREE);
  dxl.setGoalPosition(DXL_ID[3], Startpos[2], UNIT_DEGREE);
  //GoTo3D(270,140,200);

  positionGripper(1);

  delay(1000);
}
