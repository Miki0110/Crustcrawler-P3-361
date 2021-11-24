boolean gripperPosition = false;

void toggleGripper() {
  positionGripper(gripperPosition);
  gripperPosition = !gripperPosition;
}

void positionGripper(boolean pos) {
  const int gripperOpen = 1954;
  const int gripperClosed = 2020;
  
  if (pos == 0) { //Closed
    dxl.setGoalPosition(DXL_ID[4], gripperClosed, UNIT_RAW);
    dxl.setGoalPosition(DXL_ID[5], gripperClosed, UNIT_RAW);
  }
  else { //Opened
    dxl.setGoalPosition(DXL_ID[4], gripperOpen, UNIT_RAW);
    dxl.setGoalPosition(DXL_ID[5], gripperOpen, UNIT_RAW);
  }
  gripperPosition = pos;
}
