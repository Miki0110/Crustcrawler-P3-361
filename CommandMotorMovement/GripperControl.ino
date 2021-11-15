
/*
0 = 2048
closed pos ~2020
open pos ~ 1954

 
 */
int Open=1954;
int Closed=2020;


//Orient arm
void Gripper(boolean position){

if(position==0){ //Closed
dxl.setGoalPosition(DXL_ID[4], Closed, UNIT_RAW);
dxl.setGoalPosition(DXL_ID[5], Closed, UNIT_RAW);
}
else{ //Opened
dxl.setGoalPosition(DXL_ID[4], Open, UNIT_RAW);
dxl.setGoalPosition(DXL_ID[5], Open, UNIT_RAW);
  
  }
}
