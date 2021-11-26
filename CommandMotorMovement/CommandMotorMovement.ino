
#include <DynamixelShield.h>
#include <SoftwareSerial.h>
#include <BasicLinearAlgebra.h>
#include <math.h>
#include <Wire.h>

SoftwareSerial soft_serial(7, 8); // DYNAMIXELShield UART RX/TX
#define DEBUG_SERIAL soft_serial

//Motor IDs
const uint8_t DXL_ID[6] = {0, 1, 2, 3, 4, 5};

const float DXL_PROTOCOL_VERSION = 2.0;
DynamixelShield dxl(Serial3);

//PWM-Limit value
float PWMlimit = 855.0;

//Namespace for motor control table entries
using namespace ControlTableItem;

//const int millisBetweenDataSend = 10;
//unsigned long currentDataSendMillis = 0;
double starttime;
double theta[3];
double dtheta[3];
double ddtheta[3];

void setup() {
  Wire.begin();
  // Set Port baudrate to 57600bps. This has to match with DYNAMIXEL baudrate.
  dxl.begin(57600);
  // Set Port Protocol Version. This has to match with DYNAMIXEL protocol version.
  dxl.setPortProtocolVersion(DXL_PROTOCOL_VERSION);

  //Begins serial for debug and testing
  Serial.begin(57600);
  while (!Serial); //Wait for serial port to be available
  //Initialise motor control modes
  startupPWM(DXL_ID[1]);
  startupPWM(DXL_ID[2]);
  startupPWM(DXL_ID[3]);
  //startupPosition(DXL_ID[4]);
  //startupPosition(DXL_ID[5]);

  //Set start PWM
  dxl.writeControlTableItem(GOAL_PWM, DXL_ID[1], 0);
  dxl.writeControlTableItem(GOAL_PWM, DXL_ID[2], 0);
  dxl.writeControlTableItem(GOAL_PWM, DXL_ID[3], 0);

  //Orient arm
  //dxl.setGoalPosition(DXL_ID[1], 190, UNIT_DEGREE);
  //dxl.setGoalPosition(DXL_ID[2], 180, UNIT_DEGREE);
  //dxl.setGoalPosition(DXL_ID[3], 180, UNIT_DEGREE);
  starttime = millis();
}

void loop() {
  //all angles must be in rawdata!!!
  //Angles = 0.088 deg || Velocity = 0.114 rpm
  int Thetaref[3] = {0, 0, 0} / 0.088;
  int dThetaref[3] = {0, 0, 0} / 0.114;
  int ddThetaref[3] = {0, 0, 0} / 0.144;

  callPWM(Thetaref,  dThetaref, ddThetaref);
}
