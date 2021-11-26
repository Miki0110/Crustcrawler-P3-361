//Sets current from torque
boolean setMotorTorque(uint8_t id, float torque) {
  int current;

  if(id == 1 || id == 3) {
    //float MX64Kt = 0.0018189; //Torque constant for MX-64 motors (unit N*m/mA)
    float MX64Kt = 0.00110; //read on graph (unit N*m/mA)
    
    current = (int)((torque / MX64Kt)) / 3.36; //Calculate current from torque (in milliamperes)
    dxl.writeControlTableItem(GOAL_CURRENT, id, current);
    return 1;
  }
  else if(id == 2) {
    //for some reason the calculated motor constant works better than the graph one
    float MX106Kt = 0.00254650; //Torque constant for MX-106 motors (unit N*m/mA)
    //float MX106Kt = 0.00164; //read on graph (unit N*m/mA)
    
    current = (int)((torque / MX106Kt)) / 3.36; //Calculate current from torque (in milliamperes)
    dxl.writeControlTableItem(GOAL_CURRENT, id, current);
    return 1;
  }
  else {
    //Improper id called
    return 0;
  }
}

void setPWM(uint8_t id, float torque){
  float c1;
  float c2;
  float velocity = getvelocity(id);
  
 switch(id){
  case 2:
    if(velocity > 0){
    c1=138.81;
    c2=220.69;
  }else if(velocity = 0){
    c1=(138.81+40.4)/2;
    c2=220.69;    
    }else{
    c1=40.4;
    c2=220.69;
      }
  break;
  default:
    if(velocity > 0){
    c1=253.75;
    c2=173.97;
  }else if(velocity = 0){
    c1=(253.75+80.9)/2;
    c2=173.9;    
    }else{
    c1=80.9;
    c2=173.97;
      }
  break;
 }
  
  float PWM=torque*c1+velocity*c2;
  Serial.print("HELP HERE: ");
  Serial.println(PWM);
  dxl.writeControlTableItem(GOAL_PWM, id, PWM);
}
