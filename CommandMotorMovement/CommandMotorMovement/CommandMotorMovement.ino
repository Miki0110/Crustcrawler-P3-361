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

BLA::Matrix<1,3> theta0 = {0,0,-90}; //start position
BLA::Matrix<1,3> thetaf = {60,80,-50}; //end position

double speed_mod=0.03;
double max_vel=(0.114*1023)*6*speed_mod;
double tf = sqrt(pow((theta0(0,0)-thetaf(0,0)),2)+pow((theta0(0,1)-thetaf(0,1)),2)+pow((theta0(0,2)-thetaf(0,2)),2))/(max_vel);
double a=8.583*254*(speed_mod/5); //max acceleration for the MX-64

const int millisBetweenDataSend = 10;
unsigned long currentDataSendMillis = 0;
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
  while(!Serial); //Wait for serial port to be available
  //Initialise motor control modes
  startupCurrent(DXL_ID[1]);
  delay(100);
  startupCurrent(DXL_ID[2]);
  delay(100);
  startupCurrent(DXL_ID[3]);
  //delay(100);
  //startupPosition(DXL_ID[4]);
  //delay(100);
  //startupPosition(DXL_ID[5]);

  //Set start torques
  dxl.writeControlTableItem(GOAL_CURRENT, DXL_ID[1], 0);
  dxl.writeControlTableItem(GOAL_CURRENT, DXL_ID[2], 0);
  dxl.writeControlTableItem(GOAL_CURRENT, DXL_ID[3], 0);

  //Orient arm
  //dxl.setGoalPosition(DXL_ID[2], 180, UNIT_DEGREE);
  //dxl.setGoalPosition(DXL_ID[3], 180, UNIT_DEGREE);
  starttime = millis();
}

void loop() {
  //Simulate time for trajectory planning
  double t=(millis()-starttime)/1000; //just to simulate time

  if(t<tf){
    Serial.println("dav3!!!!!");
    //calculate the first theta
    theta[0]=(cubicPolyPos(t,theta0(0,0),thetaf(0,0),max_vel,tf))*PI/180;
    dtheta[0]=(cubicPolyVel(t,theta0(0,0),thetaf(0,0),max_vel,tf))*PI/180;
    ddtheta[0]=(cubicPolyAcc(t,theta0(0,0),thetaf(0,0),max_vel,tf))*PI/180;
  Serial.println("dav4!!!!!");
    //second theta
    theta[1]=(cubicPolyPos(t,theta0(0,1),thetaf(0,1),max_vel,tf))*PI/180;
    dtheta[1]=(cubicPolyVel(t,theta0(0,1),thetaf(0,1),max_vel,tf))*PI/180;
    ddtheta[1]=(cubicPolyAcc(t,theta0(0,1),thetaf(0,1),max_vel,tf))*PI/180;

    //third theta
    theta[2]=(cubicPolyPos(t,theta0(0,2),thetaf(0,2),max_vel,tf))*PI/180;
    dtheta[2]=(cubicPolyVel(t,theta0(0,2),thetaf(0,2),max_vel,tf))*PI/180;
    ddtheta[2]=(cubicPolyAcc(t,theta0(0,2),thetaf(0,2),max_vel,tf))*PI/180;
  }
  
  //calculate torques
  //                        id      theta  dtheta   ddtheta
  double tau1=computeTorque(1.0,    theta, dtheta,  ddtheta);
  double tau2=computeTorque(2.0,    theta, dtheta,  ddtheta);
  double tau3=computeTorque(3.0,    theta, dtheta,  ddtheta);
  Serial.print("tau1: ");
  Serial.print(tau1);
  Serial.print(" tau2: ");
  Serial.print(tau2);
  Serial.print(" tau3: ");
  Serial.println(tau3);
  //Control through torque
  //                id      torque
  setMotorTorque(DXL_ID[1], tau1);
  delay(10);
  setMotorTorque(DXL_ID[2], tau2);
  delay(10);
  setMotorTorque(DXL_ID[3], tau3);
  //delay(1000);
}
