
int Open = 170;   //170 degree
int Closed = 178; //178 degree
boolean Gripperposition = false;
int LeanedAngle = 0;

void ActivateGripper() {
  Gripper(Gripperposition);
  Gripperposition = !Gripperposition;
}


//Orient arm
void Gripper(boolean position) {

  if (position == 0) { //Closed
    dxl.setGoalPosition(DXL_ID[4], Closed - LeanedAngle, UNIT_DEGREE);
    dxl.setGoalPosition(DXL_ID[5], Closed + LeanedAngle, UNIT_DEGREE);
  }
  else { //Opened
    dxl.setGoalPosition(DXL_ID[4], Open - LeanedAngle, UNIT_DEGREE);
    dxl.setGoalPosition(DXL_ID[5], Open + LeanedAngle, UNIT_DEGREE);

  }
  Gripperposition = position;
}

void UpdateGripper() {
  if (Gripperposition  == 0) { //Closed
    dxl.setGoalPosition(DXL_ID[4], Closed - LeanedAngle, UNIT_DEGREE);
    dxl.setGoalPosition(DXL_ID[5], Closed + LeanedAngle, UNIT_DEGREE);
  }
  else { //Opened
    dxl.setGoalPosition(DXL_ID[4], Open - LeanedAngle, UNIT_DEGREE);
    dxl.setGoalPosition(DXL_ID[5], Open + LeanedAngle, UNIT_DEGREE);

  }
}



void GraspPiece() {
  Move(0, 0, -20);
  ActivateGripper();
  Move(0, 0, 20);

}
