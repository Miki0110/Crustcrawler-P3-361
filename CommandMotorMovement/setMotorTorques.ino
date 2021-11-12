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
    //float MX106Kt = 0.00254650; //Torque constant for MX-106 motors (unit N*m/mA)
    float MX106Kt = 0.00164; //read on graph (unit N*m/mA)
    
    current = (int)((torque / MX106Kt)) / 3.36; //Calculate current from torque (in milliamperes)
    dxl.writeControlTableItem(GOAL_CURRENT, id, current);
    return 1;
  }
  else {
    //Improper id called
    return 0;
  }
}
