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

int16_t oldPos_d[3] = {10000000, 10000000, 1000000}; //reserved for desired positions
float Theta_d[3];
float old_Theta[3];
float tf = 0.0;

//idk why i have to do this
BLA::Matrix<1, 3> cubicPolyAll(float current_time, float theta0, float thetaf, float time_f) { //function for the cubic polynomial (All)
  float a0 = theta0;
  float a1 = 0;
  float a2 = 3 / (time_f * time_f) * (thetaf - theta0);
  float a3 = -(2 / (time_f * time_f * time_f)) * (thetaf - theta0);
  BLA::Matrix<1, 3> ReturnAll = {a0 + a1 * current_time + a2 * current_time * current_time + a3 * current_time * current_time * current_time, //Pos
                                 a1 + 2 * a2 * current_time + 3 * a3 * current_time * current_time, //Vel
                                 2 * a2 + 6 * a3 * current_time //Acc
                                };
  return ReturnAll;//Pos, Vel, Acc
} //End of cubicPolyAll()

bool arrayCompare(int16_t *a, int16_t *b, int len_a, int len_b){ //simple arraycomparer
      if (len_a != len_b) return false;
      for(int n=0;n<len_a;n++) if (a[n]!=b[n]) return false;
      return true;
}

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
 #if DEBUG
  Serial.begin(57600);
  while (!Serial); //Wait for serial port to be available
  #endif

  Serial1.begin(57600); //This is the port that is reading from the arduino
  while (!Serial1); //Wait for serial port to be available

  //Start serial port for sending commands
  soft_serial.begin(57600);
  while (!soft_serial);

  PRINT("start");
  startTime = millis();
}

void loop() {
  // put your main code here, to run repeatedly:
  int16_t pwmValue[3];
  float speed_mod = 0.03;
  float max_vel = (0.114 * 1023) * 6 * speed_mod;
  
 if (millis() - startTime >= 1) {//for some reason if there's no wait time it won't read input
  if (readInput(5) == true) {
    float Thetaref[3], dThetaref[3], ddThetaref[3];
    float curTheta[3], curDTheta[3];
    
    //Convert from raw to degrees
    for (int i = 0; i < 3; i++) {
      curTheta[i] = rawcurTheta[i] * 0.088;
      curDTheta[i] = rawcurDTheta[i] * 0.114 * 6;
    }
    curDTheta[2] = rawcurDTheta[2] * 0.229 * 6; //The third motor has a different constant
    
    // check to see if the Tf needs to update
    if (arrayCompare(Pos_d, oldPos_d,3,3) == false) {
      //Find the desired angles
      setCartesianPosition(Pos_d[0], Pos_d[1], Pos_d[2]);
      
      PRINT("Desired pos");
      PRINT(Theta_d[0]);
      PRINT(Theta_d[1]);
      PRINT(Theta_d[2]);

      tf = sqrt(pow((curTheta[0] - Theta_d[0]), 2) + pow((curTheta[1] - Theta_d[1]), 2) + pow((curTheta[1] - Theta_d[0]), 2)) / (max_vel) + millis() / 1000;
      for (int i = 0; i < 3; i++) { //save the original start pos
        old_Theta[i] = curTheta[i];
        oldPos_d[i]=Pos_d[i];
      }
    }
    //Update the Theta_refs
    float t = millis() / 1000;
    BLA::Matrix<1, 3> Theta_ref;
    if (tf >= t) {
      for (int i = 0; i < 3; i++) {
        Theta_ref = cubicPolyAll(t, old_Theta[i], Theta_d[i], tf);
        Thetaref[i] = Theta_ref(0, 0);
        dThetaref[i] = Theta_ref(0, 1);
        ddThetaref[i] = Theta_ref(0, 2);
      }
    } else {
      for (int i = 0; i < 3; i++) {
        Thetaref[i] = Theta_d[i];
        dThetaref[i] = 0;
        ddThetaref[i] = 0;
      }
    }
    
    PRINT("\nTheta refs: ");
    PRINT_VALUE("\n",Thetaref[0]);
    PRINT_VALUE("\n",Thetaref[1]);
    PRINT_VALUE("\n",Thetaref[2]);
    
    torqueCalc(Thetaref, dThetaref, ddThetaref, curTheta, curDTheta);
    for (int i = 0; i < 3; i++) {
      pwmValue[i] = PWMcalc(i + 1, Q(0, i), curDTheta[i]);
    }
    PRINT_VALUE(" PWM1:\t",pwmValue[0]);
    PRINT_VALUE(" PWM2:\t",pwmValue[1]);
    PRINT_VALUE(" PWM3:\t",pwmValue[2]);

    writeOutput(pwmValue);
  }
      startTime = startTime + 1;
}
}
