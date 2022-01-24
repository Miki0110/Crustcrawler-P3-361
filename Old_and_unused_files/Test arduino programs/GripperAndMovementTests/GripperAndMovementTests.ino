#include <DynamixelShield.h>
#include <SoftwareSerial.h>
#include <BasicLinearAlgebra.h>
#include <math.h>

/*
MOTOR MAX/MIN Values
BLA::Matrix<2,5> MotorMaxMin = {1800, 2500, //Motor1
                                750, 3320,//Motor2
                                740, 3320,//Motor3
                                750, 2450,//Motor4
                                700, 2450};//Motor5
*/

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
/*
startupCurrent : You cannot yse SetGoallPosition
startupPosition : If you want to sent/SetGoalPosition
*/

  //Initialise motor control modes
  startupPosition(DXL_ID[1]);  //Base
  startupPosition(DXL_ID[2]);  //"Shoulder"
  startupPosition(DXL_ID[3]);  //"Elbow"
  startupPosition(DXL_ID[4]); //"Left gripper"
  startupPosition(DXL_ID[5]); //"Right gripper"

  //Set start torques
  //dxl.writeControlTableItem(GOAL_CURRENT, DXL_ID[1], 0);
  //dxl.writeControlTableItem(GOAL_CURRENT, DXL_ID[2], 0);
  //dxl.writeControlTableItem(GOAL_CURRENT, DXL_ID[3], 0);

  //Orient arm
//  dxl.setGoalPosition(DXL_ID[2], 180, UNIT_DEGREE);
  //dxl.setGoalPosition(DXL_ID[3], 180, UNIT_DEGREE);
  GoToStartPos();
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
//dxl.setGoalPosition(DXL_ID[1], 2100, UNIT_RAW);
//dxl.setGoalPosition(DXL_ID[2], 2600, UNIT_RAW);
//dxl.setGoalPosition(DXL_ID[3], 3000, UNIT_RAW);
  ActivateGripper(); //Function to activation of gripper
  Serial.println();
  delay(5000);
}
