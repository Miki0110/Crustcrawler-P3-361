#include <DynamixelShield.h>
#include <SoftwareSerial.h>

//Motor IDs
const uint8_t DXL_ID[6] = {0, 1, 2, 3, 4, 5};

const float DXL_PROTOCOL_VERSION = 2.0;
DynamixelShield dxl;

//Namespace for motor control table entries
using namespace ControlTableItem;

//Returns current position as radiants
float getMotorPositions(uint8_t id) {
  dxl.get
}

//TODO find torque constants
//Sets current from torque profile
boolean setMotorTorque(uint8_t id, float torque) {
  switch(id){
    case 1: //MX-64
    case 3:
      float MX64Kt; //Torque constant for MX-64 motors
      float current = (torque / MX64Kt) / 1000; //Calculate current from torque (in milliamperes)
      dxl.setGoalCurrent(id, current, UNIT_MILLI_AMPERE);
      return 1;
    
    case 2: //MX-106
      float MX106Kt; //Torque constant for MX-106 motors
      float current = (torque / MX106Kt) / 1000; //Calculate current from torque (in milliamperes)
      dxl.setGoalCurrent(id, current, UNIT_MILLI_AMPERE);
      return 1;

    /* Cannot run in torque mode
    case 4: //MX-28
    case 5:
      float MX28Kt; //Torque constant for MX-28 motors
      float current = (torque / MX28Kt) / 1000; //Calculate current from torque (in milliamperes)
      dxl.setGoalCurrent(id, current, UNIT_MILLI_AMPERE);
      return 1;
    */
    default:
      //Error wrong id
      return 0;
  }
  
}

void setup() {
  // put your setup code here, to run once:

  // Set Port baudrate to 57600bps. This has to match with DYNAMIXEL baudrate.
  dxl.begin(57600);
  // Set Port Protocol Version. This has to match with DYNAMIXEL protocol version.
  dxl.setPortProtocolVersion(DXL_PROTOCOL_VERSION);
  
  //Set maximum current (percentage)
  //setCurrentLimit(DXL_ID[1], 0);
  //setCurrentLimit(DXL_ID[2], 1000);//Range 0 - 1941
  //setCurrentLimit(DXL_ID[3], 0);
  //setCurrentLimit(DXL_ID[4], 0);
  //setCurrentLimit(DXL_ID[5], 0);

  //Orient arm
  //startupPosition(DXL_ID[2]);
  //startupPosition(DXL_ID[3]);
  //dxl.setGoalPosition(DXL_ID[2], 4095/2);
  //dxl.setGoalPosition(DXL_ID[3], 4095/2);

  //delay(1000);
  
  //Initialise motor control modes
  //startupCurrent(DXL_ID[1]);
  //startupCurrent(DXL_ID[2]);
  //startupCurrent(DXL_ID[3]);
  //startupPosition(DXL_ID[4]);
  //startupPosition(DXL_ID[5]);
}

void loop() {
  // put your main code here, to run repeatedly:

}
