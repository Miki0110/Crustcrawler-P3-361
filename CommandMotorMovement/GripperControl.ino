boolean gripperPosition = false;
int LeanedAngle = 0;

void toggleGripper() {
  positionGripper(gripperPosition);
  gripperPosition = !gripperPosition;
}

void positionGripper(boolean pos) {
  const int gripperOpen = 170; //Degree
  const int gripperClosed = 178;//Degree
  
  if (pos == 0) { //Closed
    dxl.setGoalPosition(DXL_ID[4], gripperClosed- LeanedAngle, UNIT_DEGREE);
    dxl.setGoalPosition(DXL_ID[5], gripperClosed+ LeanedAngle, UNIT_DEGREE);
  }
  else { //Opened
    dxl.setGoalPosition(DXL_ID[4], gripperOpen- LeanedAngle, UNIT_DEGREE);
    dxl.setGoalPosition(DXL_ID[5], gripperOpen+ LeanedAngle, UNIT_DEGREE);
  }
  gripperPosition = pos;
}
