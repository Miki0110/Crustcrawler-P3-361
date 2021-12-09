#include <SoftwareSerial.h>
#include <BasicLinearAlgebra.h>
#include <math.h>
#include "CRC8.h"
#include "CRC.h"

// Debugging switches and macros
#define DEBUG 1 // Switch debug output on and off by 1 or 0

#if DEBUG
#define PRINT(s)   { Serial.print(F(s)); }
#define PRINT_VALUE(s,v)  { Serial.print(F(s)); Serial.print(v); }
#define PRINT_DEC(s,v) { Serial.print(F(s)); Serial.print(v, DEC); }
#define PRINT_HEX(s,v) { Serial.print(F(s)); Serial.print(v, HEX); }
#else
#define PRINTS(s)
#define PRINT_VALUE(s,v)
#define PRINT_DEC(s,v)
#define PRINT_HEX(s,v)
#endif



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
  
 #if DEBUG
 Serial.begin(57600);
 while (!Serial); //Wait for serial port to be available
#endif
 
  Serial1.begin(57600); //This is the port that is reading from the arduino
  while (!Serial1); //Wait for serial port to be available

  //Start serial port for sending commands
  soft_serial.begin(57600);
  while(!soft_serial);


  PRINT("start");
  startTime = millis();
}

void loop() {
  // put your main code here, to run repeatedly:
  int16_t pwmValue[3];
  if (millis() - startTime >= 1) {
    if (readInput(5) == true) {
      float Thetaref[3], dThetaref[3], ddThetaref[3];
      float curTheta[3], curDTheta[3];
            
      for (int i = 0; i < 3; i++) {
        Thetaref[i] = rawThetaref[i] * 0.088;
        dThetaref[i] = rawdThetaref[i] * 0.114 *6 ;
        ddThetaref[i] = rawddThetaref[i] * 0.114 * 6;

        curTheta[i] = rawcurTheta[i] * 0.088;
        curDTheta[i] = rawcurDTheta[i] * 0.114 * 6;
      }
        curDTheta[2] = rawcurDTheta[2] * 0.229 * 6;
      
        PRINT_VALUE("\n theta1:\t",rawcurTheta[0]);
        PRINT_VALUE(" theta2:\t",rawcurTheta[1]);
        PRINT_VALUE(" theta3:\t",rawcurTheta[2]);

      
      torqueCalc(Thetaref, dThetaref, ddThetaref, curTheta, curDTheta);
      for (int i = 0; i < 3; i++) {
        pwmValue[i] = PWMcalc(i+1, Q(0, i), dThetaref[i]);
      }
        /*PRINT_VALUE(" PWM1:\t",pwmValue[0]);
        PRINT_VALUE(" PWM2:\t",pwmValue[1]);
        PRINT_VALUE(" PWM3:\t",pwmValue[2]);*/
        
      writeOutput(pwmValue);
    }
    startTime = startTime + 1;
}
}
