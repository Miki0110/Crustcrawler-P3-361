#include <DynamixelShield.h>
#include <SoftwareSerial.h>

//Motor IDs
const uint8_t DXL_ID[6] = {0, 1, 2, 3, 4, 5};

const float DXL_PROTOCOL_VERSION = 2.0;
DynamixelShield dxl;

void setup() {
  // put your setup code here, to run once:

  //Set baudrate of DYNAMIXEL port
  dxl.begin(57600);

  // Set Port baudrate to 57600bps. This has to match with DYNAMIXEL baudrate.
  dxl.begin(57600);
  // Set Port Protocol Version. This has to match with DYNAMIXEL protocol version.
  dxl.setPortProtocolVersion(DXL_PROTOCOL_VERSION);

  //Initialise motor control modes
  startupCurrent(DXL_ID[1]);
  startupCurrent(DXL_ID[2]);
  startupCurrent(DXL_ID[3]);
  startupPosition(DXL_ID[4]);
  startupPosition(DXL_ID[5]);
}

void loop() {
  // put your main code here, to run repeatedly:

}
