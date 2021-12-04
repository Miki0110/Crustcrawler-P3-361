//Control only through torque (recommended for use with dynamics)
//MX-106 and MX-64
boolean startupCurrent(uint8_t id){
  dxl.ping(id);
  dxl.torqueOff(id);
  dxl.writeControlTableItem(OPERATING_MODE, id, 0);
  dxl.torqueOn(id);
}

//Control only through PWM torque (recommended for use with dynamics)
//MX-106 and MX-64
boolean startupPWM(uint8_t id){
  while(dxl.ping(id)!=1);
  while(dxl.torqueOff(id)!=1);
  while(dxl.setOperatingMode(id, OP_PWM)!= 1);
  //dxl.writeControlTableItem(OPERATING_MODE, id, 16);
  dxl.torqueOn(id);
}


//MX-106, MX-64 and MX-28
boolean startupVelocity(uint8_t id){
  dxl.ping(id);
  dxl.torqueOff(id);
  dxl.writeControlTableItem(OPERATING_MODE, id, 1);
  dxl.torqueOn(id);
}

//MX-106, MX-64 and MX-28
boolean startupPosition(uint8_t id){
  while(dxl.ping(id)!=1);
  while(dxl.torqueOff(id)!=1);
  while(dxl.setOperatingMode(id, OP_POSITION)!= 1);
  while(dxl.torqueOn(id)!=1);
}

//Control through position, with torque limits available 
//(Allows for position control with more compliance as links cam be moved if more torque than expected is required)
//MX-106 and MX-64
boolean startupCurrentPosition(uint8_t id){
  dxl.ping(id);
  dxl.torqueOff(id);
  dxl.writeControlTableItem(OPERATING_MODE, id, 5);
  dxl.torqueOn(id);
}