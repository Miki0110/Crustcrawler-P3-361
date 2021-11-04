//MX-106 and MX-64
boolean startupCurrent(uint8_t id){
  dxl.ping(id);
  dxl.torqueOff(id);
  dxl.setOperatingMode(id, 0);
  dxl.torqueOn(id);
}

//MX-106, MX-64 and MX-28
boolean startupVelocity(uint8_t id){
  dxl.ping(id);
  dxl.torqueOff(id);
  dxl.setOperatingMode(id, 1);
  dxl.torqueOn(id);
}

//MX-106, MX-64 and MX-28
boolean startupPosition(uint8_t id){
  dxl.ping(id);
  dxl.torqueOff(id);
  dxl.setOperatingMode(id, 3);
  dxl.torqueOn(id);
}

//MX-106 and MX-64
boolean startupCurrentPosition(uint8_t id){
  dxl.ping(id);
  dxl.torqueOff(id);
  dxl.setOperatingMode(id, 5);
  dxl.torqueOn(id);
}
