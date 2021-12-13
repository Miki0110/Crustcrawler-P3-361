
#include <DynamixelShield.h>
#include <SoftwareSerial.h>
#include <BasicLinearAlgebra.h>
#include <math.h>
#include <Wire.h>
#include "CRC8.h"
#include "CRC.h"
#include "RunningAverage.h"

// Debugging switches and macros
#define DEBUG 1 // Switch debug output on and off by 1 or 0
#define DEBUG_TIME 0 // Switch debug of time output on and off by 1 or 0
#define DEBUG_BYTE 0

#if DEBUG
#define PRINT(s)   { Serial.print(F(s)); }
#define PRINT_VALUE(s,v)  { Serial.print(F(s)); Serial.print(v); }
#define PRINT_DEC(s,v) { Serial.print(F(s)); Serial.print(v, DEC); }
#define PRINT_HEX(s,v) { Serial.print(F(s)); Serial.print(v, HEX); }
#else
#define PRINT(s)
#define PRINT_VALUE(s,v)
#define PRINT_DEC(s,v)
#define PRINT_HEX(s,v)
#endif
#if DEBUG_TIME
#define t_PRINT(s) { Serial.print(F(s)); }
#define t_PRINT_VALUE(s,v) { Serial.print(F(s)); Serial.print(v); }
#define t_PRINT_DEC(s,v) { Serial.print(F(s)); Serial.print(v, DEC); }
#define t_PRINT_HEX(s,v) { Serial.print(F(s)); Serial.print(v, HEX); }
#else
#define t_PRINT(s)
#define t_PRINT_VALUE(s,v)
#define t_PRINT_DEC(s,v)
#define t_PRINT_HEX(s,v)
#endif
#if DEBUG_BYTE
#define b_PRINT(s) { Serial.print(F(s)); }
#define b_PRINT_VALUE(s,v) { Serial.print(F(s)); Serial.print(v); }
#define b_PRINT_DEC(s,v) { Serial.print(F(s)); Serial.print(v, DEC); }
#define b_PRINT_HEX(s,v) { Serial.print(F(s)); Serial.print(v, HEX); }
#else
#define b_PRINT(s)
#define b_PRINT_VALUE(s,v)
#define b_PRINT_DEC(s,v)
#define b_PRINT_HEX(s,v)
#endif


SoftwareSerial soft_serial(12, 13); // DYNAMIXELShield UART RX/TX

//Motor IDs
const uint8_t DXL_ID[6] = {0, 1, 2, 3, 4, 5};

const float DXL_PROTOCOL_VERSION = 2.0;
DynamixelShield dxl(Serial3);
CRC8 crc; //for crc checks

int counter = 0; //Debug counter (delete later)

//PWM-Limit value
float PWMlimit = 500.0; //A limit to stop the Control system from applying too high PWM values


//Namespace for motor control table entries
using namespace ControlTableItem;

unsigned long starttime;
int16_t nonError[6];

//For smoothing out the measured velocity
RunningAverage velAverage1(20);
RunningAverage velAverage2(20);
RunningAverage velAverage3(20);

int d_pos[3] = {0, 0, 10};

void setup() {
  // Set Port baudrate to 1mbps. This has to match with DYNAMIXEL baudrate.
  dxl.begin(1000000);
  // Set Port Protocol Version. This has to match with DYNAMIXEL protocol version.
  dxl.setPortProtocolVersion(DXL_PROTOCOL_VERSION);

  //Begins serial for debug and testing
  #if DEBUG || DEBUG_TIME
  Serial.begin(1000000);
  while (!Serial); //Wait for serial port to be available
  #endif
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

  delay(600);//startup delay to minimize errors
  starttime = millis();
}

void loop() {
  
  unsigned long start = millis();
   counter = 0;
  for(int i = 0; i < 100;){
if(millis() - starttime >= 20){
  callPWM(d_pos);
  starttime = starttime+20; //delay is to synchronise the entire system
  i++;
  }
}
unsigned long slut = millis();

t_PRINT_VALUE("\n Time to finish one calc: ",(slut - start)/100);
t_PRINT_VALUE("\n Times failed: ", counter);

}
