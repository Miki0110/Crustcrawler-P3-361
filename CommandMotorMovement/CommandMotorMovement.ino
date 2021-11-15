#include <DynamixelShield.h>
#include <SoftwareSerial.h>
#include <BasicLinearAlgebra.h>
#include <math.h>

SoftwareSerial soft_serial(7, 8); // DYNAMIXELShield UART RX/TX
#define DEBUG_SERIAL soft_serial

boolean Gripperpos=true;

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
  startupCurrent(DXL_ID[1]);  //Base
  startupCurrent(DXL_ID[2]);  //"Shoulder"
  startupCurrent(DXL_ID[3]);  //"Elbow"
  startupPosition(DXL_ID[4]); //"Left gripper"
  startupPosition(DXL_ID[5]); //"Right gripper"

  //Set start torques
  dxl.writeControlTableItem(GOAL_CURRENT, DXL_ID[1], 0);
  dxl.writeControlTableItem(GOAL_CURRENT, DXL_ID[2], 0);
  dxl.writeControlTableItem(GOAL_CURRENT, DXL_ID[3], 0);

  //Orient arm
  //dxl.setGoalPosition(DXL_ID[2], 180, UNIT_DEGREE);
  //dxl.setGoalPosition(DXL_ID[3], 180, UNIT_DEGREE);
}

const int millisBetweenDataSend = 10;
unsigned long currentDataSendMillis = 0;

void loop() {
  //Control through torque
  //                id      torque
  //setMotorTorque(DXL_ID[3], 1.420);
  //delay(1000);
  //setMotorTorque(DXL_ID[2], -1.420);
  //delay(1000);

  //Test of gripper
  Gripper(Gripperpos); //move the gripper (Open close)
  Gripperpos=!Gripperpos; //Change next movement
  Serial.println();
  Serial.println(Gripperpos);
  delay(2000);
}
