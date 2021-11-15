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

BLA::Matrix<1,3> theta0 = {10,10,10}; //start position
BLA::Matrix<1,3> thetaf = {60,80,-50}; //end position

double speed_mod=0.03;
double max_vel=(0.114*1023)*6*speed_mod;
double tf = sqrt(pow((theta0(0,0)-thetaf(0,0)),2)+pow((theta0(0,1)-thetaf(0,1)),2)+pow((theta0(0,2)-thetaf(0,2)),2))/(max_vel);
double a=8.583*254*(speed_mod/5); //max acceleration for the MX-64

const int millisBetweenDataSend = 10;
unsigned long currentDataSendMillis = 0;
double starttime;

void setup() {
  // Set Port baudrate to 57600bps. This has to match with DYNAMIXEL baudrate.
  dxl.begin(57600);
  // Set Port Protocol Version. This has to match with DYNAMIXEL protocol version.
  dxl.setPortProtocolVersion(DXL_PROTOCOL_VERSION);

  //Begins serial for debug and testing
  Serial1.begin(57600);
  while(!Serial1); //Wait for serial port to be available

  //Initialise motor control modes
  startupCurrent(DXL_ID[1]);
  startupCurrent(DXL_ID[2]);
  startupCurrent(DXL_ID[3]);
  startupPosition(DXL_ID[4]);
  startupPosition(DXL_ID[5]);

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
    double cTheta1 = cubicPolyPos(t,theta0(0,0),thetaf(0,0),max_vel,tf);
    double cTheta2Pos = cubicPolyPos(t,theta0(0,1),thetaf(0,1),max_vel,tf);
    double cTheta2Acc = cubicPolyAcc(t,theta0(0,1),thetaf(0,1),max_vel,tf);
    double cTheta3 = cubicPolyPos(t,theta0(0,2),thetaf(0,2),max_vel,tf);
    double tau2 = 0.0008361140800*sin(cTheta2Pos)*cos(PI/2)-0.0008361140800*cos(cTheta2Pos)*sin(PI/2)+0.6031447339*sin(cTheta2Pos)*sin(PI/2)+0.6031447339*cos(cTheta2Pos)*cos(PI/2)+(0.6524125717*sin(cTheta2Pos)*cos(PI/2)-0.6524125717*cos(cTheta2Pos)*sin(PI/2)-0.1934061093*cos(cTheta2Pos)*cos(PI/2)-0.1934061093*sin(cTheta2Pos)*sin(PI/2)+0.008793848148)*cTheta2Acc;

    
    double pTheta1 = paraBlend(t,theta0(0,0),thetaf(0,0),max_vel,a,tf);
    double pTheta2 = paraBlend(t,theta0(0,1),thetaf(0,1),max_vel,a,tf);
    double pTheta3 = paraBlend(t,theta0(0,2),thetaf(0,2),max_vel,a,tf);
  }
  

  //Control through torque
  //                id      torque
  //setMotorTorque(DXL_ID[3], 1.420);
  //delay(1000);
  //setMotorTorque(DXL_ID[2], -1.420);
  //delay(1000);
}
