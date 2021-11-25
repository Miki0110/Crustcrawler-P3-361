#include <DynamixelShield.h>
#include <SoftwareSerial.h>
#include <BasicLinearAlgebra.h>
#include <math.h>
BLA::Matrix<3, 3> TrajectoryGeneration(double newx, double newy, double newz); //DO NOT REMOVE!! IS NEEDED TO USE THE FUNCTION TrajectoryGeneration
//LCD
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 16, 2);
//LCD uses SDA (Serial data) and SCL (Serial Clock) pins! (Uno = A4,A5)(Mega = pin 20, pin 21)


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
DynamixelShield dxl;

//Namespace for motor control table entries
using namespace ControlTableItem;

//Desired cartesian positions
double desiredXPos = 140;
double desiredYPos = 140;
double desiredZPos = 140;

double movementStep = 10; //How much to increment the value of an axis each time a command is received

int calculationInterval = 100; //ms between movement calculations
unsigned long lastCalcTime;


//Returns joint angle as radiants
float getMotorPosition(uint8_t id) {
  return (PI / 180) * dxl.getPresentPosition(id, UNIT_DEGREE);
}

//Returns joint velocity in radiants/s
float getMotorVelocity(uint8_t id) {
  return (1 / 60) * 2 * PI * dxl.getPresentVelocity(id, UNIT_RPM);
}

void setup() {
  lcd.begin(); //16 columns and 2 rows on the LCD
  lcd.backlight(); //activate the baacklight for the LCD
  
  // Set Port baudrate to 57600bps. This has to match with DYNAMIXEL baudrate.
  dxl.begin(57600);
  // Set Port Protocol Version. This has to match with DYNAMIXEL protocol version.
  dxl.setPortProtocolVersion(DXL_PROTOCOL_VERSION);

  //Begins serial for debug and testing
  Serial1.begin(57600);
  while (!Serial1); //Wait for serial port to be available
  /*
    startupCurrent : You cannot use SetGoallPosition
    startupPosition : If you want to sent/SetGoalPosition
  */

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
  GoToStartPos();

  lastCalcTime = millis();
}

void loop() {
  receivedInputsFromSerial();

  if (millis() >= lastCalcTime + calculationInterval) {
    GoTo(desiredXPos, desiredYPos, desiredZPos);
    BLA::Matrix<3, 3> Final = TrajectoryGeneration(desiredXPos, desiredYPos, desiredZPos);
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
    LCDPrint();
}
