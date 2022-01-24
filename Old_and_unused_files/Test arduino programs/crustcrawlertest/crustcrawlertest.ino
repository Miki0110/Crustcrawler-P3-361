#include <DynamixelShield.h>

//#if defined(ARDUINO_AVR_UNO) || defined(ARDUINO_AVR_MEGA2560)
  #include <SoftwareSerial.h>
  SoftwareSerial soft_serial(7, 8); // DYNAMIXELShield UART RX/TX
  #define DEBUG_SERIAL soft_serial

const uint8_t DXL_ID1 = 1;
const uint8_t DXL_ID2 = 2;
const uint8_t DXL_ID3 = 3;
const uint8_t DXL_ID4 = 4;
const uint8_t DXL_ID5 = 5;
const float DXL_PROTOCOL_VERSION = 2.0;

unsigned long previousMillis = 0; 

DynamixelShield dxl;

//This namespace is required to use Control table item names
using namespace ControlTableItem;

boolean setStartup(uint8_t id){
  dxl.ping(id);
  dxl.torqueOff(id);
  dxl.setOperatingMode(id, OP_POSITION);
  dxl.torqueOn(id);
}

boolean setSpeed(float speedPCT,double unit, uint8_t id) {
  int VELOCITY_LIMIT = 1023;
  double VELOCITY_UNIT = unit; // rpm
  double maxSpeed = VELOCITY_LIMIT * VELOCITY_UNIT;
  uint32_t newSpeed = speedPCT * maxSpeed; // rpm
  uint32_t writeTimeout = 100; // ms
  dxl.writeControlTableItem(PROFILE_VELOCITY, id, newSpeed, writeTimeout);
}

void setup() {
  // put your setup code here, to run once:
  
  // For Uno, Nano, Mini, and Mega, use UART port of DYNAMIXEL Shield to debug.
  DEBUG_SERIAL.begin(115200);

  // Set Port baudrate to 57600bps. This has to match with DYNAMIXEL baudrate.
  dxl.begin(57600);
  // Set Port Protocol Version. This has to match with DYNAMIXEL protocol version.
  dxl.setPortProtocolVersion(DXL_PROTOCOL_VERSION);

  setStartup(DXL_ID1);
  setStartup(DXL_ID2);
  setStartup(DXL_ID3);
  setStartup(DXL_ID4);
  setStartup(DXL_ID5);

  setSpeed(0.2,0.229,DXL_ID1);
  setSpeed(0.2,0.229,DXL_ID2);
  setSpeed(0.2,0.229,DXL_ID3);
  setSpeed(0,05.114,DXL_ID4);
  setSpeed(0,05.114,DXL_ID5);
  
  dxl.setGoalPosition(DXL_ID3, 4095/2);
  dxl.setGoalPosition(DXL_ID2, 4095/2);

}

void loop() {
  unsigned long currentMillis = millis();
  // put your main code here, to run repeatedly:
  
  // Please refer to e-Manual(http://emanual.robotis.com/docs/en/parts/interface/dynamixel_shield/) for available range of value. 
  // Set Goal Position in RAW value
  dxl.ledOn(DXL_ID1);
  dxl.ledOn(DXL_ID2);
  dxl.ledOn(DXL_ID3);
  dxl.ledOn(DXL_ID4);
  dxl.ledOn(DXL_ID5);
  dxl.setGoalPosition(DXL_ID1, 0);
  dxl.setGoalPosition(DXL_ID2, 4095/1.5);
  dxl.setGoalPosition(DXL_ID3, 4095/2.5);
  delay(100);
  dxl.setGoalPosition(DXL_ID4, 4095/2+400);
  dxl.setGoalPosition(DXL_ID5, 4095/2-400);
  delay(2000);
  dxl.ledOff(DXL_ID1);
  dxl.ledOff(DXL_ID2);
  dxl.ledOff(DXL_ID3);
  dxl.ledOff(DXL_ID4);
  dxl.ledOff(DXL_ID5);
  dxl.setGoalPosition(DXL_ID1, 1500);
  dxl.setGoalPosition(DXL_ID3, 4095/1.5);
  dxl.setGoalPosition(DXL_ID2, 4095/2.5);
  delay(100);
  dxl.setGoalPosition(DXL_ID4, 4095/2-400);
  dxl.setGoalPosition(DXL_ID5, 4095/2+400);
  // Print present position in raw value
  DEBUG_SERIAL.print("Present Position(raw) : ");
  DEBUG_SERIAL.println(dxl.getPresentPosition(DXL_ID2));
  delay(2000);

  // Set Goal Position in DEGREE value
 // dxl.setGoalPosition(DXL_ID, 5.7, UNIT_DEGREE);
 // delay(1000);
  // Print present position in degree value
 // DEBUG_SERIAL.print("Present Position(degree) : ");
 // DEBUG_SERIAL.println(dxl.getPresentPosition(DXL_ID, UNIT_DEGREE));
 // delay(1000);
}
