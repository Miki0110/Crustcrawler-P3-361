#include <DynamixelShield.h>
#include <SoftwareSerial.h>
#include <BasicLinearAlgebra.h>
#include <math.h>

SoftwareSerial soft_serial(7, 8); // DYNAMIXELShield UART RX/TX
#define DEBUG_SERIAL soft_serial

//Motor IDs
const uint8_t DXL_ID[6] = {0, 1, 2, 3, 4, 5};

const float DXL_PROTOCOL_VERSION = 2.0;
DynamixelShield dxl;

//Namespace for motor control table entries
using namespace ControlTableItem;

//Returns joint angle as radiants
float getMotorPosition(uint8_t id) {
  return PI/180 * dxl.getPresentPosition(id, UNIT_DEGREE);
}

//Returns joint velocity in radiants/s
float getMotorVelocity(uint8_t id) {
  return (1/60) * 2*PI * dxl.getPresentVelocity(id, UNIT_RPM);
}

void setup() {
  // Set Port baudrate to 57600bps. This has to match with DYNAMIXEL baudrate.
  dxl.begin(57600);
  // Set Port Protocol Version. This has to match with DYNAMIXEL protocol version.
  dxl.setPortProtocolVersion(DXL_PROTOCOL_VERSION);

  //Begins serial for debug and testing
  Serial1.begin(57600);
  while(!Serial1); //Wait for serial port to be available

  //Initialise motor control modes
  startupCurrentPosition(DXL_ID[1]);
  startupCurrentPosition(DXL_ID[2]);
  startupCurrentPosition(DXL_ID[3]);
  //startupPosition(DXL_ID[4]);
  //startupPosition(DXL_ID[5]);

  //Set start torques
  dxl.writeControlTableItem(GOAL_CURRENT, DXL_ID[1], 0);
  dxl.writeControlTableItem(GOAL_CURRENT, DXL_ID[2], 0);
  //dxl.writeControlTableItem(GOAL_CURRENT, DXL_ID[3], 0);

  //Orient arm
  //dxl.setGoalPosition(DXL_ID[2], 180, UNIT_DEGREE);
  dxl.setGoalPosition(DXL_ID[3], 180, UNIT_DEGREE);
}

float desiredTor = 0.8;//N*m
float desiredVel = 0;  //deg/s
float desiredPos = 90; //deg

const int millisBetweenDataSend = 10;
unsigned long currentDataSendMillis = 0;

void loop() {
  //Information to debug arduino
  if(millis() >= currentDataSendMillis + millisBetweenDataSend) {
    int cur = (int)(dxl.getPresentCurrent(DXL_ID[2]) * 100);
    int vel = dxl.getPresentVelocity(DXL_ID[2]);
    int pos = dxl.getPresentPosition(DXL_ID[2]);
    
    Serial1.write(0x7E);
    Serial1.write(highByte(cur));
    Serial1.write(lowByte(cur));
    Serial1.write(highByte(vel));
    Serial1.write(lowByte(vel));
    Serial1.write(highByte(pos));
    Serial1.write(lowByte(pos));
    currentDataSendMillis = millis();
  }

  //control loop for motor

  
}
