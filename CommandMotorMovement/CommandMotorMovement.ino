
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
CRC8 crc; //for crc checks

int counter = 0; //Debug counter (delete later)

//PWM-Limit value
float PWMlimit = 855.0; //A limit to stop the Control system from applying too high PWM values


//Namespace for motor control table entries
using namespace ControlTableItem;

//const int millisBetweenDataSend = 10;
//unsigned long currentDataSendMillis = 0;
unsigned long starttime;
unsigned long starttime2;
double theta[3];
double dtheta[3];
double ddtheta[3];

int d_pos[3] = {0, 0, 15};

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
  starttime2 = millis();
}

void loop() {
d_pos[2] = 10;
  /*if(millis() < starttime2){
    
    }else if(millis() < starttime2+150){
    d_pos[1] = 100;
    }else if(millis() < starttime2+300){
    d_pos[1] = 200;
    }else if(millis() < starttime2+350){
    d_pos[1] = 240;
    }else if(millis() < starttime2+500){
    d_pos[0] = 100;
    }else if(millis() < starttime2+650){
    d_pos[0] = 200;
    }else if(millis() < starttime2+700){
    d_pos[0] = 240;
    }else if(millis() < starttime2+850){
    d_pos[1] = 200;
    }else if(millis() < starttime2+1000){
    d_pos[1] = 100;
    }else if(millis() < starttime2+1050){
    d_pos[1] = 0;
    }else if(millis() < starttime2+1200){
    d_pos[0] = 200;
    }else if(millis() < starttime2+1350){
    d_pos[0] = 100;
    }else if(millis() < starttime2+1400){
    d_pos[0] = 0;
    }else {
    starttime2=millis();
    }*/

  
  unsigned long start = millis();
   counter = 0;
  //for(int i = 0; i < 100;){
if(millis() - starttime >= 20){
  callPWM(d_pos);
  starttime = starttime+20; //delay is to synchronise the entire system

//}
}
unsigned long slut = millis();

}
