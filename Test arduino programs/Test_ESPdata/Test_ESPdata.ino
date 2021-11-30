#include <SoftwareSerial.h>
#include <BasicLinearAlgebra.h>
#include <math.h>
#include "CRC8.h"
#include "CRC.h"

SoftwareSerial soft_serial(32, 14); // RX/TX
CRC8 crc;

unsigned long startTime;
//Bytes to send
byte startByte = 0xDF;
int PWMcommand;
BLA::Matrix<1, 3> Q;

//Bytes to recieve
byte startRByte = 0x5A;

float rawThetaref[3];
float rawdThetaref[3];
float rawddThetaref[3];

float rawcurTheta[3];
float rawcurDTheta[3];



void setup() {
  // put your setup code here, to run once:
  Serial.begin(57600);
  while (!Serial); //Wait for serial port to be available

  Serial1.begin(57600); //This is the port that is reading from the arduino
  while (!Serial1); //Wait for serial port to be available

  //Start serial port for sending commands
  soft_serial.begin(57600);


  Serial.println("start");
  startTime = millis();
}

void loop() {
  // put your main code here, to run repeatedly:
  int pwmValue[3];
  if (millis() - startTime >= 1) {
    if (readInput(100) == true) {
      float Thetaref[3], dThetaref[3], ddThetaref[3];
      float curTheta[3], curDTheta[3];
      
      for (int i = 0; i < 3; i++) {
        Thetaref[i] = rawThetaref[i] * 0.088;
        dThetaref[i] = rawdThetaref[i] * 0.114 * 360 / 60;
        ddThetaref[i] = rawddThetaref[i] * 0.114 * 360 / 60;

        curTheta[i] = rawcurTheta[i] * 0.088;
        curDTheta[i] = rawcurDTheta[i] * 0.114 * 360 / 60;
      }

      torqueCalc(Thetaref, dThetaref, ddThetaref, curTheta, curDTheta);
      for (int i = 0; i < 3; i++) {
        pwmValue[i] = PWMcalc(i, Q(0, i), curDTheta);
      }
      writeOutput(pwmValue);
    }
    startTime = startTime + 1;
  }
}
