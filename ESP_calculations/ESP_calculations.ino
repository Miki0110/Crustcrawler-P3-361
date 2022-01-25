#include <SoftwareSerial.h>
#include <BasicLinearAlgebra.h>
#include <math.h>
#include "CRC8.h"
#include "CRC.h"


// Debugging switches and macros
#define DEBUG 1 // Switch debug output on and off by 1 or 0
#define DEBUG_BYTE 0 // Switch debug output on and off by 1 or 0

#if DEBUG
#define PRINT(s)   { Serial.print(F(s)); }
#define PRINT_VALUE(s,v)  { Serial.print(F(s)); Serial.print(v); }
#define PRINT_DEC(s,v) { Serial.print(F(s)); Serial.print(v, DEC); }
#define PRINT_HEX(s,v) { Serial.print(F(s)); Serial.print(v, HEX); }
#else
#define PRINT(s)
#define PRINT_VALUE(s,v)
#define PRINT_DEC(s,v)
#define PRINT_HEX(s,v)
#endif
#if DEBUG_BYTE
#define b_PRINT(s) { Serial.print(F(s)); }
#define b_PRINT_VALUE(s,v) { Serial.print(F(s)); Serial.print(v); }
#define b_PRINT_DEC(s,v) { Serial.print(F(s)); Serial.print(v, DEC); }
#define b_PRINT_HEX(s,v) { Serial.print(F(s)); Serial.print(v, HEX); }
#else
#define b_PRINT(s)
#define b_PRINT_VALUE(s,v)
#define b_PRINT_DEC(s,v)
#define b_PRINT_HEX(s,v)
#endif



SoftwareSerial soft_serial(32, 14); // RX/TX
CRC8 crc;

//idk why i have to do this
BLA::Matrix<1, 3> cubicPolyAll(unsigned long current_time, float theta0, float thetaf, unsigned long time_f);
BLA::Matrix<1, 3> setCartesianPosition(int16_t X, int16_t Y, int16_t Z);

bool arrayCompare(int16_t *a, int16_t *b, int len_a, int len_b) { //simple arraycomparer
  if (len_a != len_b) return false;
  for (int n = 0; n < len_a; n++) if (a[n] != b[n]) return false;
  return true;
}

unsigned long startTime;
//Bytes to send
byte startByte1 = 0x42;
byte startByte2 = 0xDF;

//Bytes to recieve
byte startRByte1 = 0x41;
byte startRByte2 = 0xDF;

//Saved variables
BLA::Matrix<1, 3> Q;

int16_t rawcurTheta[3];
int16_t rawcurDTheta[3];
int16_t Pos_d[3];


int16_t Posold_d[3]; //reserved for desired positions
float old_Theta[3];
unsigned long tf = 0.0;
      


void setup() {
#if DEBUG || DEBUG_BYTE
  Serial.begin(57600);
  while (!Serial); //Wait for serial port to be available
#endif

  Serial1.begin(57600); //This is the port that is reading from the arduino
  while (!Serial1); //Wait for serial port to be available

  //Start serial port for sending commands
  soft_serial.begin(57600);
  while (!soft_serial);

  for (int i = 0; i < 3; i++) {
    Posold_d[i] = 10000;
  }
  PRINT("start");
  startTime = millis();
}

void loop() {
  // put your main code here, to run repeatedly:
  int16_t pwmValue[3];
  float speed_mod = 0.01;
  float max_vel = (0.114 * 1023) * 0.006 * speed_mod;

  if (millis() - startTime >= 1) {//for some reason if there's no wait time it won't read input
    if (readInput(5) == true) {
      //Defining variables
      float Thetaref[3], dThetaref[3], ddThetaref[3];
      float curTheta[3], curDTheta[3];
      
      //Convert from raw to degrees, degrees/s
      for (int i = 0; i < 3; i++) {
        curTheta[i] = (rawcurTheta[i] * 0.088) - 180; //Minus 180 so it fits with the snadard setup from kinematics
        curDTheta[i] = rawcurDTheta[i] * 0.114 * 6;
      }
      curDTheta[1] = rawcurDTheta[1] * 0.229 * 6; //The second motor has a different constant

      //Find the desired angles
       BLA::Matrix<1, 3> Theta_d = setCartesianPosition(Pos_d[0], Pos_d[1], Pos_d[2]);

      // check to see if the Tf needs to update
      if (arrayCompare(Pos_d, Posold_d, 3, 3) == false) {
        tf = sqrt(pow((curTheta[0] - Theta_d(0, 0)), 2) + pow((curTheta[1] - Theta_d(0, 1)), 2) + pow((curTheta[2] - Theta_d(0, 2)), 2)) / (max_vel) + millis();
        for (int i = 0; i < 3; i++) { //save the original start pos
          old_Theta[i] = curTheta[i];
          Posold_d[i] = Pos_d[i];
        }
      }

      //Update the Theta_refs
      unsigned long t = millis();

      BLA::Matrix<1, 3> Theta_ref;
      
      if (tf >= t) {
        for (int i = 0; i < 3; i++) {
          Theta_ref = cubicPolyAll(t, old_Theta[i], Theta_d(0, i), tf);
          Thetaref[i] = Theta_ref(0, 0);
          dThetaref[i] = Theta_ref(0, 1);
          ddThetaref[i] = Theta_ref(0, 2);
        }
      } else {
        for (int i = 0; i < 3; i++) {
          Thetaref[i] = Theta_d(0, i);
          dThetaref[i] = 0;
          ddThetaref[i] = 0;
        }
      }
     /* PRINT_VALUE("\n D1\t",Theta_d(0,0));
      PRINT_VALUE(" D2\t",Theta_d(0,1));
      PRINT_VALUE(" D3\t",Theta_d(0,2));
      PRINT_VALUE(" Theta1\t",Thetaref[0]);
      PRINT_VALUE(" Theta2\t",Thetaref[1]);
      PRINT_VALUE(" Theta3\t",Thetaref[2]);*/
      
      torqueCalc(Thetaref, dThetaref, ddThetaref, curTheta, curDTheta);
      for (int i = 0; i < 3; i++) {
        pwmValue[i] = PWMcalc(i + 1, Q(0, i), dThetaref[i]);
      }

      writeOutput(pwmValue);
    }
    startTime = startTime + 1;
  }
}
