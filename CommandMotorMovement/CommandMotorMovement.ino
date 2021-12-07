#include <DynamixelShield.h>
#include <SoftwareSerial.h>
#include <BasicLinearAlgebra.h>
#include <math.h>

//Running average library by Rob Tillaart (Version 0.4.1)
#include <RunningAverage.h>

//Define rolling averages and sample amounts (keep sample amounts low (<50))
RunningAverage sEMGch1(25);
RunningAverage sEMGch2(25);

BLA::Matrix<3, 3> TrajectoryGeneration(double newx, double newy, double newz); //DO NOT REMOVE!! IS NEEDED TO USE THE FUNCTION TrajectoryGeneration

SoftwareSerial soft_serial(7, 8); // DYNAMIXELShield UART RX/TX
#define DEBUG_SERIAL soft_serial

//Motor IDs
const uint8_t DXL_ID[6] = {0, 1, 2, 3, 4, 5};

const float DXL_PROTOCOL_VERSION = 2.0;
DynamixelShield dxl(Serial3);

//Namespace for motor control table entries
using namespace ControlTableItem;

//Threshholds for signals from sEMG channels to be counted as active
const int sEMGch1Threshold = 120; //Orbiclaris oculi 75% = 135
const int sEMGch2Threshold = 120; //Frontalis 75% = 120

//Time threshold for an sEMG channel signal to be counted as a held signal
const int timeForHold = 100; //unit is sEMGInterpreterSampleTime in ms

//Desired cartesian positions alter these to pick a starting position (units are mm)
double desiredXPos = 140.00;
double desiredYPos = 140.00;
double desiredZPos = 70.00;

//Current axis tracker og indicator light pins
byte currentAxis = 1; //1 = X, 2 = Y, 3 = Z
const byte pinR = 23, pinHI = 25, pinG = 27, pinB = 29; //LED RBG & GND pins

//Limits to cartesian positions
double HiLimitXPos =  350.00;
double LoLimitXPos =   -5.00;
double HiLimitYPos =  300.00 + 280.00;
double LoLimitYPos = -300.00;
double HiLimitZPos =  700.00;
double LoLimitZPos =    0.00;

//How much to increment the value of an axis each time a command is received
double movementStep = 0.50; //Steps are in mm

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

  //Orient arm
  //  dxl.setGoalPosition(DXL_ID[2], 180, UNIT_DEGREE);
  //dxl.setGoalPosition(DXL_ID[3], 180, UNIT_DEGREE);
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

  //Prepare interpreiter timer for use
  sEMGInterpreterTime = millis();

  //Prepare calculator timer for use
  lastCalcTime = millis();
}

void loop() {
  /*
    Serial.println("Starts motorCalling");
    lastCalcTime = millis();
    for (int antal = 1; antal <= 1000; antal++) {
    for (int i = 1; i <= 3; i++) {
      dxl.getPresentPosition(DXL_ID[i], UNIT_DEGREE);
    };
    };
    Serial.print("Took ");
    Serial.print(millis() - lastCalcTime);
    Serial.println(" ms");
  */

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

  if (millis() >= lastCalcTime + calculationInterval) {
    GoTo(desiredXPos, desiredYPos, desiredZPos);

    //Record calculation time
    lastCalcTime += calculationInterval;
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
