boolean startupCurrent(uint8_t id){
  dxl.ping(id);
  dxl.torqueOff(id);
  dxl.setOperatingMode(id, OP_CURRENT);
  dxl.torqueOn(id);
}

boolean startupVelocity(uint8_t id){
  dxl.ping(id);
  dxl.torqueOff(id);
  dxl.setOperatingMode(id, OP_VELOCITY);
  dxl.torqueOn(id);
}

boolean startupPosition(uint8_t id){
  dxl.ping(id);
  dxl.torqueOff(id);
  dxl.setOperatingMode(id, OP_POSITION);
  dxl.torqueOn(id);
}
