boolean setSpeedLimit(float speedPct,double unit, uint8_t id) {
  int VELOCITY_LIMIT = 1023;
  double VELOCITY_UNIT = unit; // rpm
  double maxSpeed = VELOCITY_LIMIT * VELOCITY_UNIT;
  uint32_t newSpeed = speedPct * maxSpeed; // rpm
  uint32_t writeTimeout = 100; // ms
  dxl.writeControlTableItem(PROFILE_VELOCITY, id, newSpeed, writeTimeout);
}

boolean setAccelerationLimit(float speedPCT,double unit, uint8_t id) {
  int VELOCITY_LIMIT = 1023;
  double VELOCITY_UNIT = unit; // rpm
  double maxSpeed = VELOCITY_LIMIT * VELOCITY_UNIT;
  uint32_t newSpeed = speedPCT * maxSpeed; // rpm
  uint32_t writeTimeout = 100; // ms
  dxl.writeControlTableItem(PROFILE_VELOCITY, id, newSpeed, writeTimeout);
}

boolean setCurrentLimit(uint32_t currentLimit, uint8_t id) {
  dxl.ping(id);
  dxl.torqueOff(id);
  dxl.writeControlTableItem(CURRENT_LIMIT, id, currentLimit);
  dxl.torqueOn(id);
}
