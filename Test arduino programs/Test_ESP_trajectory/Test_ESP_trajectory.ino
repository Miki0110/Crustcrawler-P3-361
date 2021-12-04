#include <SoftwareSerial.h>
#include <BasicLinearAlgebra.h>
#include <math.h>
#include "CRC8.h"
#include "CRC.h"

SoftwareSerial soft_serial(32, 14); // RX/TX
CRC8 crc;

int16_t oldPos_d[3]; //reserved for desired positions
float Theta_d[3];
float old_Theta[3];
float tf = 0.0;

unsigned long startTime;
//Bytes to send
byte startByte1 = 0x42;
byte startByte2 = 0xDF;
int PWMcommand;
BLA::Matrix<1, 3> Q;

//Bytes to recieve
byte startRByte1 = 0x41;
byte startRByte2 = 0xDF;

int16_t Pos_d[3];
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
  float speed_mod = 0.1;
  float max_vel = (0.114 * 1023) * 6 * speed_mod;

  
    if (readInput(5) == true) {
      float Thetaref[3], dThetaref[3], ddThetaref[3];
      float curTheta[3], curDTheta[3];

      //Convert from raw to degrees
      for (int i = 0; i < 3; i++) {
        curTheta[i] = rawcurTheta[i] * 0.088;
        curDTheta[i] = rawcurDTheta[i] * 0.114 * 6;
      }
  // check to see if the Tf needs to update
  if(Pos_d != oldPos_d){
      //Find the desired angles
      setCartesianPosition(Pos_d[0],Pos_d[1],Pos_d[2]);
     tf = sqrt(pow((curTheta[0] - Theta_d[0]), 2) + pow((curTheta[1] - Theta_d[1]), 2) + pow((curTheta[1] - Theta_d[0]), 2)) / (max_vel) + millis()/1000;
     for(int i=0;i<3;i++){ //save the original start pos
       old_Theta[i] = curTheta[i];
     }
    }
  //Update the Theta_refs
  float t = millis()/1000;
  BLA::Matrix<1, 3> Theta_ref;
  for (int i = 0; i < 3; i++) {
  Theta_ref = cubicPolyAll(t, old_Theta[i], Theta_d[i], max_vel, tf);
  Thetaref[i] = Theta_ref(0, 0);
  dThetaref[i] = Theta_ref(0, 1);
  ddThetaref[i] = Theta_ref(0, 2);
  }
      
      torqueCalc(Thetaref, dThetaref, ddThetaref, curTheta, curDTheta);
      for (int i = 0; i < 3; i++) {
        pwmValue[i] = PWMcalc(i+1, Q(0, i), curDTheta[i]);
      }
      
      writeOutput(pwmValue);
    }
}
