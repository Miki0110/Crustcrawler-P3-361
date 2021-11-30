#include <SoftwareSerial.h>
#include "CRC8.h"
#include "CRC.h"

SoftwareSerial soft_serial(7, 8); // DYNAMIXELShield UART RX/TX
CRC8 crc;

unsigned long startTime;
double theta[3];
double dtheta[3];
double ddtheta[3];


void setup() {
  // put your setup code here, to run once:
  Serial1.begin(57600);
  Serial.begin(57600);
  while (!Serial); //Wait for serial port to be available
  soft_serial.begin(57600);

  Serial.println("start");
  startTime = millis();
}

void loop() {
  // put your main code here, to run repeatedly:
  
//all angles must be in rawdata!!!
  //Angles = 0.088 deg || Velocity = 0.114 rpm
  int Thetaref[3] = {0, 0, 0};// / 0.088;
  int dThetaref[3] = {0, 0, 0};// / 0.114;
  int ddThetaref[3] = {0, 0, 0};// / 0.144;
//Serial.println("start");
//Serial.println(Serial.read());
if(millis() - startTime >= 4){
  callPWM(Thetaref,  dThetaref, ddThetaref);
  startTime = startTime+4; //delay is to avoid potential bit overrides
}
}
