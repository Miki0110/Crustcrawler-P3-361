#include <DynamixelShield.h>
#include <SoftwareSerial.h>
#include <BasicLinearAlgebra.h>
#include <math.h>

//Running average library by Rob Tillaart
#include <RunningAverage.h>

//Define rolling averages and sample amounts (keep sample amounts low (<50))
RunningAverage sEMGch1(25);
RunningAverage sEMGch2(25);

/*
  MOTOR MAX/MIN Values (UNIT_RAW)
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
DynamixelShield dxl(Serial3);

//Namespace for motor control table entries
using namespace ControlTableItem;

//Threshholds for signals from sEMG channels to be counted as active
const int sEMGch1Threshold = 535; //Orbiclaris oculi 75% = 135
const int sEMGch2Threshold = 520; //Frontalis 75% = 120

//Time threshold for an sEMG channel signal to be counted as a held signal
const int timeForHold = 100; //unit is sEMGInterpreterSampleTime in ms

//Desired cartesian positions alter these to pick a starting position (units are mm)
double desiredXPos = 140;
double desiredYPos = 140;
double desiredZPos = 70;

//How much to increment the value of an axis each time a command is received
double movementStep = 0.1; //Steps are in mm

//----Timings----
const int calculationInterval = 100; //ms between movement calculations
unsigned long lastCalcTime;

const int sEMGInterpreterSampleTime = 10; //Time between interpreter mesurements in miliseconds (minimum should be 10)
unsigned long sEMGInterpreterTime;  //Counter to keep track of milliseconds between the interpreiter sample collection


//----Data saving----
int sEMGFetchedData[5];      //Array for storing newest data fetched from sEMG
byte interpretedCommand = 0; //Variable for storing the data interpreted command recieved from sEMG

//Returns joint angle as radiants
float getMotorPosition(uint8_t id) {
  return (PI / 180) * dxl.getPresentPosition(id, UNIT_DEGREE);
}

//Returns joint velocity in radiants/s
float getMotorVelocity(uint8_t id) {
  return (1 / 60) * 2 * PI * dxl.getPresentVelocity(id, UNIT_RPM);
}

void setup() {
  // Set Port baudrate to 57600bps. This has to match with DYNAMIXEL baudrate.
  dxl.begin(57600);
  // Set Port Protocol Version. This has to match with DYNAMIXEL protocol version.
  dxl.setPortProtocolVersion(DXL_PROTOCOL_VERSION);

  Serial.begin(57600);

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

  //Orient arm
  //  dxl.setGoalPosition(DXL_ID[2], 180, UNIT_DEGREE);
  //dxl.setGoalPosition(DXL_ID[3], 180, UNIT_DEGREE);
  setSpeedLimit(0.4, 0.114, DXL_ID[1]);
  setSpeedLimit(0.4, 0.229, DXL_ID[2]);
  setSpeedLimit(0.4, 0.114, DXL_ID[3]);
  //GoToStartPos();

  //Ensure clean slate for all rolling averages
  sEMGch1.clear();
  sEMGch2.clear();

  //Prepare interpreiter timer for use
  sEMGInterpreterTime = millis();

  //Prepare calculator timer for use
  lastCalcTime = millis();
}

void loop() {
  //Fetch data from sEMG
  fetchDataFromsEMG(100);

  //Run sEMG signal interpreiter
  if (millis() >= sEMGInterpreterTime + sEMGInterpreterSampleTime) {
    sEMGInterpreter();
    sEMGInterpreterTime = millis();

    //Act according to the recieved input command
    actOnReceivedInputs(interpretedCommand);
    interpretedCommand = 0; //Reset command
    sEMGInterpreterTime = millis();
  }
  
  if (millis() >= lastCalcTime + calculationInterval) {
    GoTo(desiredXPos, desiredYPos, desiredZPos);

    //Record calculation time
    lastCalcTime = millis();
  }

  /*
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
    Gripper(0);
    Serial.println();
    for(int i=160; i<=260; i=i+3){
    GoTo2D(i,130);
    delay(10);
    }
    for(int i=130; i<=265; i=i+3){
    GoTo2D(260,i);
    delay(10);
    }
    for(int i=260; i>=160; i=i-3){
    GoTo2D(i,265);
    delay(10);
    }
    for(int i=265; i>=130; i=i-3){
    GoTo2D(160,i);
    delay(10);
    }*/
}
