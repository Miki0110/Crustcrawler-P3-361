#include <SoftwareSerial.h>
#include <BasicLinearAlgebra.h>
#include <math.h>

SoftwareSerial soft_serial(32, 14); // RX/TX

//Bytes to send
byte startByte = 0xDF;
int PWMcommand;

//Bytes to recieve
byte startRByte = 0x5A;

int Thetaref[3];
int dThetaref[3];
int ddThetaref[3];

int curTheta[3];
int curDTheta[3];

boolean readInput(int timeOut){ //insert call function to the other arduino
  byte recieverByte[32];
  long currentMillis = millis();

  while(true){
    if(currentmillis + timeOut <= millis()){
      return 0 //failed communication
      }
      if (Serial.read() == startRByte) {
        Serial.readBytes(reciverByte, 31);
 
 /*     for (int i = 31; i > 0; i--) {
        reciverByte[i] = reciverByte[i - 1];
      }*/
    for(int i = 0, i < 6, i=i+2){
    Thetaref[i]= (reciverByte[i]<<8)+reciverByte[i+1];
    dThetaref[i]= (reciverByte[i+6]<<8)+reciverByte[i+7];
    ddThetaref[i]= (reciverByte[i+12]<<8)+reciverByte[i+13];

    curThetar[i]= (reciverByte[i+18]<<8)+reciverByte[i+19];
    curDThetar[i]= (reciverByte[i+24]<<8)+reciverByte[i+25];
    }
   return 1       
      }
    }
  }

void setup() {
  // put your setup code here, to run once:
  Serial.begin(57600);
  while (!Serial); //Wait for serial port to be available

  //Start serial port for sending commands
  soft_serial.begin(57600);
  
}

void loop() {
  // put your main code here, to run repeatedly:

  PWMcalc(Thetaref,  dThetaref, ddThetaref);
}
