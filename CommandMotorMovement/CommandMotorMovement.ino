#include <DynamixelShield.h>
#include <SoftwareSerial.h>

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

//Sets current from torque
boolean setMotorTorque(uint8_t id, float torque) {
  float current;
  switch(id){
    case 1: //MX-64
    case 3:
      float MX64Kt = 1.8189; //Torque constant for MX-64 motors
      current = (torque / MX64Kt) / 1000; //Calculate current from torque (in milliamperes)
      dxl.setGoalCurrent(id, current, UNIT_MILLI_AMPERE);
      return 1;
    
    case 2: //MX-106
      float MX106Kt = 2.5465; //Torque constant for MX-106 motors
      current = (torque / MX106Kt) / 1000; //Calculate current from torque (in milliamperes)
      dxl.setGoalCurrent(id, current, UNIT_MILLI_AMPERE);
      return 1;
      
    default:
      //Error wrong id
      return 0;
  }
  
}

void setup() {
  // Set Port baudrate to 57600bps. This has to match with DYNAMIXEL baudrate.
  dxl.begin(57600);
  // Set Port Protocol Version. This has to match with DYNAMIXEL protocol version.
  dxl.setPortProtocolVersion(DXL_PROTOCOL_VERSION);

  //Begins serial for debug and testing
  Serial1.begin(57600);
  while(!Serial1); //Wait for serial port to be available

  //Orient arm
  startupPosition(DXL_ID[2]);
  startupPosition(DXL_ID[3]);
  dxl.setGoalPosition(DXL_ID[2], 90, UNIT_DEGREE);
  dxl.setGoalPosition(DXL_ID[3], 180, UNIT_DEGREE);
  
  //Initialise motor control modes
  //startupCurrent(DXL_ID[1]);
  //startupCurrent(DXL_ID[2]);
  //startupCurrent(DXL_ID[3]);
  //startupPosition(DXL_ID[4]);
  //startupPosition(DXL_ID[5]);
}

float desiredCurrent  = 0; //Unit 3.36mA
float desiredVelocity = 0; //Unit 0.229rpm
float desiredPosition = 0; //Unit 0.088deg

const int millisBetweenDataSend = 10;
unsigned long currentDataSendMillis = 0;

void loop() {
  //Information to debug arduino

  if(millis() >= currentDataSendMillis + millisBetweenDataSend) {
    int cur = (int)(dxl.getPresentCurrent(DXL_ID[2]) * 100);
    int vel = dxl.getPresentVelocity(DXL_ID[2]);
    int pos = dxl.getPresentPosition(DXL_ID[2]);
    
    Serial1.write(0x7E);
    Serial1.write(highByte(cur));
    Serial1.write(lowByte(cur));
    Serial1.write(highByte(vel));
    Serial1.write(lowByte(vel));
    Serial1.write(highByte(pos));
    Serial1.write(lowByte(pos));
    currentDataSendMillis = millis();
  }

  //control loop for motor
  
  
}
