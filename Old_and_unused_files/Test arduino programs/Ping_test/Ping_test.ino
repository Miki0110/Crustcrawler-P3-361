#include <DynamixelShield.h>
#include <SoftwareSerial.h>

#if defined(ARDUINO_AVR_UNO) || defined(ARDUINO_AVR_MEGA2560)
  #include <SoftwareSerial.h>
  SoftwareSerial soft_serial(7, 8); // DYNAMIXELShield UART RX/TX
  #define DEBUG_SERIAL soft_serial
#elif defined(ARDUINO_SAM_DUE) || defined(ARDUINO_SAM_ZERO)
  #define DEBUG_SERIAL SerialUSB    
#else
  #define DEBUG_SERIAL Serial
#endif

SoftwareSerial mySerial(7, 8);

const uint8_t DXL_ID1 = 1;
const uint8_t DXL_ID2 = 2;
const uint8_t DXL_ID3 = 3;
const uint8_t DXL_ID4 = 4;
const float DXL_PROTOCOL_VERSION = 2.0;

DynamixelShield dxl;

//This namespace is required to use Control table item names
using namespace ControlTableItem;

boolean printPing(uint8_t id){
  Serial.print("PROTOCOL ");
  Serial.print(DXL_PROTOCOL_VERSION, 1);
  Serial.print(", ID ");
  Serial.print(id);
  Serial.print(": ");
  
  if(dxl.ping(id) == true){
    Serial.print("ping succeeded!");
    Serial.print(", Model Number: ");
    Serial.println(dxl.getModelNumber(id), DEC);
  }else{
    Serial.println("ping failed!");
  }
  delay(3000);
  }

void setup() {
  // put your setup code here, to run once:
  
  // For Uno, Nano, Mini, and Mega, use UART port of DYNAMIXEL Shield to debug.
  mySerial.begin(115200);
  Serial.begin(115200);
  
  // Set Port baudrate to 57600bps. This has to match with DYNAMIXEL baudrate.
  dxl.begin(57600);
  // Set Port Protocol Version. This has to match with DYNAMIXEL protocol version.
  dxl.setPortProtocolVersion(DXL_PROTOCOL_VERSION);
}

void loop() {
  // put your main code here, to run repeatedly:
  for (uint8_t i=0; i<5; i++){
    printPing(i+1);
  }
}

  
