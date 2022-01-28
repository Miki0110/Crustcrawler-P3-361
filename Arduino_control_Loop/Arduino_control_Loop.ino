#include <DynamixelShield.h>
#include <SoftwareSerial.h>
#include <BasicLinearAlgebra.h>
#include <math.h>
//Running average & CRC library by Rob Tillaart (Version 0.4.1)
#include <RunningAverage.h>
#include "CRC8.h"
#include "CRC.h"

#define TORQUECONTROL 1 //If controlled through torque control set 1


// Debugging switches and macros
#define DEBUG 1 // Switch debug output on and off by 1 or 0

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



//Define rolling averages and sample amounts (keep sample amounts low (<50))
RunningAverage sEMGch1(25);
RunningAverage sEMGch2(25);
CRC8 crc; //for crc checks

BLA::Matrix<3, 3> TrajectoryGeneration(double newx, double newy, double newz); //DO NOT REMOVE!! IS NEEDED TO USE THE FUNCTION TrajectoryGeneration

//Motor IDs
const uint8_t DXL_ID[6] = {0, 1, 2, 3, 4, 5};
const float DXL_PROTOCOL_VERSION = 2.0;
DynamixelShield dxl(Serial3);

//Namespace for motor control table entries
using namespace ControlTableItem;

//Threshholds for signals from sEMG channels to be counted as active
const int sEMGch1Threshold = 80; //Orbiclaris oculi 75% = 135
const int sEMGch2Threshold = 200; //Frontalis 75% = 120

//Time threshold for an sEMG channel signal to be counted as a held signal
const int timeForHold = 100; //unit is sEMGInterpreterSampleTime in ms

//Desired cartesian positions alter these to pick a starting position (units are mm)
double desiredXPos = 140.00;
double desiredYPos = 140.00;
double desiredZPos = 30.00;

//Current axis tracker og indicator light pins
byte currentAxis = 1; //1 = X, 2 = Y, 3 = Z
const byte pinR = 23, pinG = 25, pinB = 27, pinHI = 29; //LED RBG & GND pins

//Limits to cartesian positions
double HiLimitXPos =   70.00 + 280.00;
double LoLimitXPos =  -70.00;
double HiLimitYPos =   70.00 + 280.00;
double LoLimitYPos =  -70.00;
double HiLimitZPos =   70.00;
double LoLimitZPos =    0.00;

//How much to increment the value of an axis each time a command is received
double movementStep = 0.50; //Steps are in mm

//----Timings----
const int calculationInterval = 50; //ms between movement calculations
unsigned long lastCalcTime;

const int sEMGInterpreterSampleTime = 10; //Time between interpreter mesurements in miliseconds (minimum should be 10)
unsigned long sEMGInterpreterTime;  //Counter to keep track of milliseconds between the interpreiter sample collection


//----Data saving----
int sEMGFetchedData[5];      //Array for storing newest data fetched from sEMG
byte interpretedCommand = 0; //Variable for storing the data interpreted command recieved from sEMG

//Returns joint angle as radians
float getMotorPosition(uint8_t id) {
  return (PI / 180) * dxl.getPresentPosition(id, UNIT_DEGREE);
}

//Returns joint velocity in radians/s
float getMotorVelocity(uint8_t id) {
  return (1 / 60) * 2 * PI * dxl.getPresentVelocity(id, UNIT_RPM);
}

//Torque control related definitions
#if TORQUECONTROL
//A limit to stop the Control system from applying too high PWM values
float PWMlimit = 500.0;
//For smoothing out the measured velocity
RunningAverage velAverage1(20);
RunningAverage velAverage2(20);
RunningAverage velAverage3(20);
//For saving data incase of errors
int16_t nonError[6];
//ESP connection UART RX/TX
SoftwareSerial soft_serial(12, 13); // DYNAMIXELShield UART RX/TX
#endif

void setup() {
  // Set Port baudrate to 1000000bps. This has to match with DYNAMIXEL baudrate.
  dxl.begin(1000000);
  // Set Port Protocol Version. This has to match with DYNAMIXEL protocol version.
  dxl.setPortProtocolVersion(DXL_PROTOCOL_VERSION);

  //Begins the Serial port for the USB for PC
  Serial.begin(1000000);

  //Begins serial port for sEMG XBee connection
  Serial1.begin(115200);
  while (!Serial1); //Wait for serial port to be available

  //Initialise motor control modes
  startupPosition(DXL_ID[1]);  //Base
  startupPosition(DXL_ID[2]);  //"Shoulder"
  startupPosition(DXL_ID[3]);  //"Elbow"
  startupPosition(DXL_ID[4]);  //"Left gripper"
  startupPosition(DXL_ID[5]);  //"Right gripper"

  //Set start torques
  //dxl.writeControlTableItem(GOAL_CURRENT, DXL_ID[1], 0);
  //dxl.writeControlTableItem(GOAL_CURRENT, DXL_ID[2], 0);
  //dxl.writeControlTableItem(GOAL_CURRENT, DXL_ID[3], 0);
  
  setSpeedLimit(0.4, 0.114, DXL_ID[1]);
  setSpeedLimit(0.4, 0.229, DXL_ID[2]);
  setSpeedLimit(0.4, 0.114, DXL_ID[3]);

  //Indicator lights
  pinMode(pinR, OUTPUT);   //Red pin
  pinMode(pinG, OUTPUT);   //Green pin
  pinMode(pinB, OUTPUT);   //Blue pin
  pinMode(pinHI, OUTPUT); //Ground pin
  digitalWrite(pinHI, HIGH);
  axisIndicator(currentAxis);

  //Ensure clean slate for all rolling averages
  sEMGch1.clear();
  sEMGch2.clear();

  //Prepare calculator timer for use
  lastCalcTime = millis();

  //Prepare interpreiter timer for use
  sEMGInterpreterTime = millis();

  //Startup Torque control
  #if TORQUECONTROL
  //--Start serial for communication----
  Serial2.begin(57600);
  while(!Serial2);
  soft_serial.begin(57600);
  while(!soft_serial);
  //--Activate PWM-control--
  startupPWM(DXL_ID[1]);
  startupPWM(DXL_ID[2]);
  startupPWM(DXL_ID[3]);
  //Set start PWM
  dxl.writeControlTableItem(GOAL_PWM, DXL_ID[1], 0);
  dxl.writeControlTableItem(GOAL_PWM, DXL_ID[2], 0);
  dxl.writeControlTableItem(GOAL_PWM, DXL_ID[3], 0);
  #endif
}

void loop() {
  //Fetch data from sEMG
  fetchDataFromsEMG(100);
  
  //Run sEMG signal interpreiter
    if (millis() >= sEMGInterpreterTime + sEMGInterpreterSampleTime) {
    sEMGInterpreter();
    //Act according to the recieved input command
    actOnReceivedInputs(interpretedCommand);
    interpretedCommand = 0; //Reset command
    sEMGInterpreterTime += sEMGInterpreterSampleTime;
  }
    #if TORQUECONTROL
    int d_pos[3];
      d_pos[0]=desiredXPos;
      d_pos[1]=desiredYPos;
      d_pos[2]=desiredZPos;
    if(millis() - lastCalcTime >= 40){
    callPWMangle(d_pos);
    lastCalcTime = lastCalcTime+40; //delay is to synchronise the entire system
  }  
    #else
  if (millis() >= lastCalcTime + calculationInterval) {
    GoTo(desiredXPos, desiredYPos, desiredZPos);

    //Record calculation time
    lastCalcTime += calculationInterval;
  }
    #endif
}
