#include <SoftwareSerial.h>
#include <BasicLinearAlgebra.h>
#include <math.h>
#include "CRC8.h"
#include "CRC.h"

SoftwareSerial soft_serial(32, 14); // RX/TX
CRC8 crc;

unsigned long startTime;
//Bytes to send
byte startByte1 = 0x42;
byte startByte2 = 0xDF;
int PWMcommand;
BLA::Matrix<1, 3> Q;

//Bytes to recieve
byte startRByte1 = 0x41;
byte startRByte2 = 0xDF;

int16_t rawThetaref[3];
int16_t rawdThetaref[3];
int16_t rawddThetaref[3];

int16_t rawcurTheta[3];
int16_t rawcurDTheta[3];



void setup() {
  // put your setup code here, to run once:
  Serial.begin(57600);
  while (!Serial); //Wait for serial port to be available

  Serial1.begin(57600); //This is the port that is reading from the arduino
  while (!Serial1); //Wait for serial port to be available

  //Start serial port for sending commands
  soft_serial.begin(57600);
  while(!soft_serial);


  Serial.println("start");
  startTime = millis();
}

void loop() {
  // put your main code here, to run repeatedly:
  int16_t pwmValue[3];
  if (millis() - startTime >= 1) {
    if (readInput(5) == true) {
      float Thetaref[3], dThetaref[3], ddThetaref[3];
      float curTheta[3], curDTheta[3];
      for(int i = 0; i < 3; i++){
      }

      
      for (int i = 0; i < 3; i++) {
        Thetaref[i] = rawThetaref[i] * 0.088;
        dThetaref[i] = rawdThetaref[i] * 0.114 *6 ;
        ddThetaref[i] = rawddThetaref[i] * 0.114 * 6;

        curTheta[i] = rawcurTheta[i] * 0.088;
        curDTheta[i] = rawcurDTheta[i] * 0.114 * 6;
      }
        Serial.print(rawcurTheta[0]);
        Serial.print(" Theta1:\t");
        Serial.print(rawcurDTheta[0]);
        Serial.print(" dTheta1:\t");
        
        Serial.print(rawcurTheta[1]);
        Serial.print(" Theta2:\t");
        Serial.print(rawcurDTheta[1]);
        Serial.print(" dTheta2:\t");

        Serial.print(rawcurTheta[3]);
        Serial.print(" Theta3:\t");
        Serial.print(rawcurDTheta[3]);
        Serial.println(" dTheta3:\t");
      
      torqueCalc(Thetaref, dThetaref, ddThetaref, curTheta, curDTheta);
      for (int i = 0; i < 3; i++) {
        pwmValue[i] = PWMcalc(i+1, Q(0, i), curDTheta[i]);
      }
      
      writeOutput(pwmValue);
    }
    startTime = startTime + 1;
  }
}
