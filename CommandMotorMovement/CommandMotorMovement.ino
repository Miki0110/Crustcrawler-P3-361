#include <DynamixelShield.h>
#include <SoftwareSerial.h>

//Motor IDs
const uint8_t DXL_ID[6] = {0, 1, 2, 3, 4, 5};

const float DXL_PROTOCOL_VERSION = 2.0;
DynamixelShield dxl;

//Namespace for motor control table entries
using namespace ControlTableItem;

void setup() {
  // put your setup code here, to run once:

  // Set Port baudrate to 57600bps. This has to match with DYNAMIXEL baudrate.
  dxl.begin(57600);
  // Set Port Protocol Version. This has to match with DYNAMIXEL protocol version.
  dxl.setPortProtocolVersion(DXL_PROTOCOL_VERSION);

  //Set maximum torque (percentage)
  setTorqueLimit(DXL_ID[1], 0);
  setTorqueLimit(DXL_ID[2], 100);
  setTorqueLimit(DXL_ID[3], 0);
  setTorqueLimit(DXL_ID[4], 0);
  setTorqueLimit(DXL_ID[5], 0);

  //Orient arm
  startupPosition(DXL_ID[2]);
  startupPosition(DXL_ID[3]);
  dxl.setGoalPosition(DXL_ID[2], 4095/2);
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
