int TopLeft[2]= {0,0};
int TopRight[2]= {0,0};
int MidLeft[2]= {0,0};
int MidRight[2]= {0,0};
int ButLeft[2]= {0,0};
int ButRight[2]= {0,0};
int route[]= {TopRight, TopLeft, MidRight, MidLeft, ButRight, ButLeft};


void FinalGripperTest(){
  Gripper(0);

for(int i=1; i<=(sizeof(route) / sizeof(int));i++){
  Serial.println("Moving to: " + route[i-1]);

//GoTo(route[i-1]);
ActivateGripper();

}
delay(1500);

for(int i=(sizeof(route) / sizeof(int)); i>=1;i--){
  Serial.println("Moving to: " + route[i-1]);

//GoTo(route[i-1]);
ActivateGripper();
}
delay(1500);

Serial.println("*-------------------*");
Serial.println("| ! Test Complete ! |");
Serial.println("*-------------------*");
}
