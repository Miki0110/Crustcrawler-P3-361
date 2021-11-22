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
  const int Startpos[] = {2171, 2500, 3000, 1954, 1954}; //Startpos for the motors

  for (int i = 1; i <= 3; i++) {
    dxl.setGoalPosition(DXL_ID[i], Startpos[i - 1], UNIT_RAW);
    delay(1000);
  }

  dxl.setGoalPosition(DXL_ID[4], Startpos[4 - 1], UNIT_RAW);
  dxl.setGoalPosition(DXL_ID[5], Startpos[5 - 1], UNIT_RAW);
  delay(1000);

  //while(d){}

}
