
#include <DynamixelShield.h>
#include <SoftwareSerial.h>
#include <BasicLinearAlgebra.h>
#include <math.h>
#include <Wire.h>
#include "CRC8.h"
#include "CRC.h"

SoftwareSerial soft_serial(12, 13); // DYNAMIXELShield UART RX/TX

//Motor IDs
const uint8_t DXL_ID[6] = {0, 1, 2, 3, 4, 5};

const float DXL_PROTOCOL_VERSION = 2.0;
DynamixelShield dxl(Serial3);
CRC8 crc;

int counter = 0;

//PWM-Limit value
float PWMlimit = 500.0;
int16_t PWMvalue[3];

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
  dxl.begin(1000000);
  // Set Port Protocol Version. This has to match with DYNAMIXEL protocol version.
  dxl.setPortProtocolVersion(DXL_PROTOCOL_VERSION);

  //Begins serial for debug and testing
  Serial.begin(1000000);
  while (!Serial); //Wait for serial port to be available
  Serial2.begin(57600);
  while(!Serial2);
  
  soft_serial.begin(57600);
  while(!soft_serial);
  
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

  pinMode(9, OUTPUT); //Debug pin
  starttime = millis();
}

void loop() {
  //all angles must be in degrees
  float Thetaref[3] = {0, 0, 0};
  float dThetaref[3] = {0, 0, 0};
  float ddThetaref[3] = {0, 0, 0};
  unsigned long start = millis();
   counter = 0;
  for(int i = 0; i < 100;){
if(millis() - starttime >= 20){
  callPWM(Thetaref,  dThetaref, ddThetaref);
  starttime = starttime+20; //delay is to synchronise the entire system
  i++;
}
}
unsigned long slut = millis();
/*Serial.println("Time to finish one calc");
Serial.println((slut - start)/100);
Serial.println("times failed");
Serial.println(counter);*/

}
