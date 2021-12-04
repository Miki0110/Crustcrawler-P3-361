#include <SoftwareSerial.h>
#include <BasicLinearAlgebra.h>
#include <math.h>
#include "CRC8.h"
#include "CRC.h"

SoftwareSerial soft_serial(32, 14); // RX/TX
CRC8 crc;

//Bytes to send
byte startByte = 0xDF;
int PWMcommand;

//Bytes to recieve
byte startRByte = 0x5A;

float Thetaref[3];
float dThetaref[3];
float ddThetaref[3];

float curTheta[3];
float curDTheta[3];

boolean readInput(int timeOut) { //insert call function to the other arduino
  byte recieverByte[32];
  long currentMillis = millis();

  int rawThetaref[3], rawdThetaref[3], rawddThetaref[3], rawcurTheta[3], rawcurDTheta[3];

  while (true) {
    if (currentMillis + timeOut <= millis()) {
      return 0; //failed communication
    }
    if (Serial.read() == startRByte) {
      Serial.readBytes(recieverByte, 31);

      crc.reset();
      crc.setPolynome(0x07);
      crc.add(startRByte);
      for (int i = 0; i < 30; i++)
      {
        crc.add(recieverByte[i]);
      }
      Serial.print("CRC calced: ");
      Serial.println(crc.getCRC(), HEX);

      Serial.print("Last byte: ");
      Serial.println(recieverByte[30], HEX);
      
      if (crc.getCRC() == recieverByte[30]) { //should check the last bit
        for (int i = 0; i < 6; i = i + 2) {
          rawThetaref[i / 2] = (recieverByte[i] << 8) + recieverByte[i + 1];
          rawdThetaref[i / 2] = (recieverByte[i + 6] << 8) + recieverByte[i + 7];
          rawddThetaref[i / 2] = (recieverByte[i + 12] << 8) + recieverByte[i + 13];

          rawcurTheta[i / 2] = (recieverByte[i + 18] << 8) + recieverByte[i + 19];
          rawcurDTheta[i / 2] = (recieverByte[i + 24] << 8) + recieverByte[i + 25];

          Thetaref[i / 2] = rawThetaref[i / 2] * 0.088;
          dThetaref[i / 2] = rawdThetaref[i / 2] * 0.114 * 360 / 60;
          ddThetaref[i / 2] = rawddThetaref[i / 2] * 0.114 * 360 / 60;

          curTheta[i / 2] = rawcurTheta[i / 2] * 0.088;
          curDTheta[i / 2] = rawcurDTheta[i / 2] * 0.114 * 360 / 60;
        }
        return 1;
      } else return 0;
    }
  }
}


/////////////////////////////////////////////////////////////////////////////////////////////////
/*/////////////////////////////////////////////////////////////////////////////////////////////////
                    Holy shit guys don't put delays in this
/////////////////////////////////////////////////////////////////////////////////////////////////*/




BLA::Matrix<3,3> computeH(float theta[3]){
//Function for calculating the mass matrix

//defining thetas
float theta1=(theta[0]-190)*PI/180, theta2=(theta[1]-180)*PI/180, theta3=(theta[2]-180)*PI/180;

//defing the mass matrix
float H11=0.00166*sin(2.0*theta3 + 2.0*theta2 - 1.0*theta1) - 0.0014*cos(theta3 + 2.0*theta1) - 0.00115*cos(theta3 + 2.0*theta2) - 0.00574*sin(- 2.0*theta2 + theta1) + 0.00574*sin(2.0*theta2 + theta1) + 0.0014*cos(theta3 + 2.0*theta2 - 2.0*theta1) + 0.0014*cos(theta3 + 2.0*theta2 + 2.0*theta1) + 0.00471*sin(theta3 + 2.0*theta2 - 1.0*theta1) + 0.00166*sin(2.0*theta3 + 2.0*theta2 + theta1) - 0.00442*cos(2.0*theta1) - 0.0014*cos(2.0*theta2) + 0.00471*sin(theta3 + 2.0*theta2 + theta1) + 0.0124*cos(theta3) + 0.00171*cos(- 2.0*theta2 + 2.0*theta1) + 0.00171*cos(2.0*theta2 + 2.0*theta1) - 0.0014*cos(- 1.0*theta3 + 2.0*theta1) + 0.0198;
float H12=0.00733*sin(- 1.0*theta3 + theta1) + 0.00733*sin(theta3 + theta1) + 0.0231*sin(theta1);
float H13=0.00366*sin(- 1.0*theta3 + theta1) + 0.00366*sin(theta3 + theta1) + 0.00521*sin(theta1);
float H21=0.00733*sin(- 1.0*theta3 + theta1) + 0.00733*sin(theta3 + theta1) + 0.0231*sin(theta1);
float H22=0.0191*cos(theta3) + 0.0302;
float H23=0.00957*cos(theta3) + 0.0068;
float H31=0.00366*sin(- 1.0*theta3 + theta1) + 0.00366*sin(theta3 + theta1) + 0.00521*sin(theta1);
float H32=0.00957*cos(theta3) + 0.0068;
float H33=0.0068;
return {H11,H12,H13,
        H21,H22,H23,
        H31,H32,H33};
}


BLA::Matrix<1,3> computeC(float theta[3], float dtheta[3]){
//Function for calculating the Coriolis effect

  
//defining thetas
float theta1=(theta[0]-190)*PI/180, theta2=(theta[1]-180)*PI/180, theta3=(theta[2]-180)*PI/180;
//velocity
float dtheta1=dtheta[0]*PI/180, dtheta2=dtheta[1]*PI/180,dtheta3=dtheta[2]*PI/180;

//defining the coriolis effect vector
float C1=0.00442*pow(dtheta1,2)*sin(2.0*theta1) - 0.0124*dtheta1*dtheta3*sin(theta3) - 0.00342*pow(dtheta1,2)*cos(2.0*theta2)*sin(2.0*theta1) + 0.00281*pow(dtheta1,2)*sin(2.0*theta1)*cos(theta3) - 0.00574*pow(dtheta1,2)*sin(2.0*theta2)*sin(theta1) + 0.00279*dtheta1*dtheta2*sin(2.0*theta2) - 0.00733*pow(dtheta3,2)*sin(theta1)*sin(theta3) - 0.00166*pow(dtheta1,2)*cos(2.0*theta2)*sin(2.0*theta3)*sin(theta1) - 0.00166*pow(dtheta1,2)*cos(2.0*theta3)*sin(2.0*theta2)*sin(theta1) + 0.00281*pow(dtheta1,2)*sin(2.0*theta1)*sin(2.0*theta2)*sin(theta3) - 0.00684*dtheta1*dtheta2*cos(2.0*theta1)*sin(2.0*theta2) - 0.00471*pow(dtheta1,2)*cos(2.0*theta2)*sin(theta1)*sin(theta3) - 0.00471*pow(dtheta1,2)*sin(2.0*theta2)*cos(theta3)*sin(theta1) + 0.023*dtheta1*dtheta2*cos(2.0*theta2)*cos(theta1) + 0.00229*dtheta1*dtheta2*cos(2.0*theta2)*sin(theta3) + 0.00229*dtheta1*dtheta2*sin(2.0*theta2)*cos(theta3) + 0.00281*dtheta1*dtheta3*cos(2.0*theta1)*sin(theta3) + 0.00115*dtheta1*dtheta3*cos(2.0*theta2)*sin(theta3) + 0.00115*dtheta1*dtheta3*sin(2.0*theta2)*cos(theta3) - 0.0147*dtheta2*dtheta3*sin(theta1)*sin(theta3) - 0.00281*pow(dtheta1,2)*cos(2.0*theta2)*sin(2.0*theta1)*cos(theta3) - 0.00662*dtheta1*dtheta2*sin(2.0*theta2)*sin(2.0*theta3)*cos(theta1) - 0.00662*dtheta1*dtheta3*sin(2.0*theta2)*sin(2.0*theta3)*cos(theta1) + 0.0188*dtheta1*dtheta2*cos(2.0*theta2)*cos(theta1)*cos(theta3) + 0.00942*dtheta1*dtheta3*cos(2.0*theta2)*cos(theta1)*cos(theta3) - 0.0188*dtheta1*dtheta2*sin(2.0*theta2)*cos(theta1)*sin(theta3) - 0.00942*dtheta1*dtheta3*sin(2.0*theta2)*cos(theta1)*sin(theta3) - 0.00197*dtheta1*dtheta2*cos(2.0*theta1)*cos(2.0*theta2)*sin(2.0*theta3) - 0.00197*dtheta1*dtheta2*cos(2.0*theta1)*cos(2.0*theta3)*sin(2.0*theta2) - 0.00197*dtheta1*dtheta3*cos(2.0*theta1)*cos(2.0*theta2)*sin(2.0*theta3) - 0.00197*dtheta1*dtheta3*cos(2.0*theta1)*cos(2.0*theta3)*sin(2.0*theta2) + 0.00662*dtheta1*dtheta2*cos(2.0*theta2)*cos(2.0*theta3)*cos(theta1) + 0.00662*dtheta1*dtheta3*cos(2.0*theta2)*cos(2.0*theta3)*cos(theta1) - 0.00561*dtheta1*dtheta2*cos(2.0*theta1)*cos(2.0*theta2)*sin(theta3) - 0.00561*dtheta1*dtheta2*cos(2.0*theta1)*sin(2.0*theta2)*cos(theta3) - 0.00281*dtheta1*dtheta3*cos(2.0*theta1)*cos(2.0*theta2)*sin(theta3) - 0.00281*dtheta1*dtheta3*cos(2.0*theta1)*sin(2.0*theta2)*cos(theta3);
float C2=0.0231*pow(dtheta1,2)*cos(theta1) - 0.0014*pow(dtheta1,2)*sin(2.0*theta2) - 0.00957*pow(dtheta3,2)*sin(theta3) - 0.0191*dtheta2*dtheta3*sin(theta3) + 0.00342*pow(dtheta1,2)*cos(2.0*theta1)*sin(2.0*theta2) - 0.0115*pow(dtheta1,2)*cos(2.0*theta2)*cos(theta1) - 0.00115*pow(dtheta1,2)*cos(2.0*theta2)*sin(theta3) - 0.00115*pow(dtheta1,2)*sin(2.0*theta2)*cos(theta3) + 0.0147*pow(dtheta1,2)*cos(theta1)*cos(theta3) + 0.00331*pow(dtheta1,2)*sin(2.0*theta2)*sin(2.0*theta3)*cos(theta1) - 0.00942*pow(dtheta1,2)*cos(2.0*theta2)*cos(theta1)*cos(theta3) + 0.00942*pow(dtheta1,2)*sin(2.0*theta2)*cos(theta1)*sin(theta3) - 0.00331*pow(dtheta1,2)*cos(2.0*theta2)*cos(2.0*theta3)*cos(theta1) - 0.0147*dtheta1*dtheta3*sin(theta1)*sin(theta3) + 0.00281*pow(dtheta1,2)*cos(2.0*theta1)*cos(2.0*theta2)*sin(theta3) + 0.00281*pow(dtheta1,2)*cos(2.0*theta1)*sin(2.0*theta2)*cos(theta3);
float C3=0.00619*pow(dtheta1,2)*sin(theta3) + 0.00957*pow(dtheta2,2)*sin(theta3) + 0.00521*pow(dtheta1,2)*cos(theta1) - 0.0014*pow(dtheta1,2)*cos(2.0*theta1)*sin(theta3) + 0.00733*pow(dtheta1,2)*cos(theta1)*cos(theta3) + 0.00331*pow(dtheta1,2)*sin(2.0*theta2)*sin(2.0*theta3)*cos(theta1) - 0.00471*pow(dtheta1,2)*cos(2.0*theta2)*cos(theta1)*cos(theta3) + 0.00471*pow(dtheta1,2)*sin(2.0*theta2)*cos(theta1)*sin(theta3) - 0.00331*pow(dtheta1,2)*cos(2.0*theta2)*cos(2.0*theta3)*cos(theta1) + 0.0147*dtheta1*dtheta2*sin(theta1)*sin(theta3) + 0.0014*pow(dtheta1,2)*cos(2.0*theta1)*cos(2.0*theta2)*sin(theta3) + 0.0014*pow(dtheta1,2)*cos(2.0*theta1)*sin(2.0*theta2)*cos(theta3);

return {C1,
        C2,
        C3};
}

BLA::Matrix<1,3> computeG(float theta[3]){
  //Function for calculating the Gravity effect

  
  //defining thetas
float theta1=(theta[0]-190)*PI/180, theta2=(theta[1]-180)*PI/180, theta3=(theta[2]-180)*PI/180;
  //calcs
float G1 = 0.0014*cos(theta1) + 0.832*sin(theta1)*sin(theta2) + 0.327*cos(theta2)*sin(theta1)*sin(theta3) + 0.327*cos(theta3)*sin(theta1)*sin(theta2);
float G2 = 0.327*cos(theta1)*sin(theta2)*sin(theta3) - 0.832*cos(theta1)*cos(theta2) - 0.275*cos(theta2)*sin(theta3) - 0.275*cos(theta3)*sin(theta2) - 0.698*sin(theta2) - 0.327*cos(theta1)*cos(theta2)*cos(theta3);
float G3 = 0.327*cos(theta1)*sin(theta2)*sin(theta3) - 0.275*cos(theta3)*sin(theta2) - 0.275*cos(theta2)*sin(theta3) - 0.327*cos(theta1)*cos(theta2)*cos(theta3);

return {G1,
        G2,
        G3};
}



float errorFunc(float measured, float desired){ //error function
  return desired - measured;
}


void updateRef(){ //Function for updating the references (should be set up with trajectory later)
  }



float errH(float H1, float H2, float H3, float ddtheta[3]){
  //acceleration
  float ddtheta1=ddtheta[0]*PI/180,ddtheta2=ddtheta[1]*PI/180,ddtheta3=ddtheta[2]*PI/180;

  return H1*ddtheta1+H2*ddtheta2+H3*ddtheta3;
  }

BLA::Matrix<1, 3> PWMcalc(float Thetaref[3], float dThetaref[3], float ddThetaref[3]) {

  //due to the difference between the used 0 values and the motors values an offset is input
  Thetaref[0] = Thetaref[0] + 191.6, Thetaref[1] = Thetaref[1] + 180, Thetaref[2] = Thetaref[2] + 180;

  //initializing variables
  float errTheta[3], errDTheta[3], errDDTheta[3];

  BLA::Matrix<1, 3> Q = {0, 0, 0};
  //float kp[3] = {16, 25, 36}; //tested kp values
  //float kd[3] = {8,  10, 12}; //tested kd values

  float kp[3] = {4, 16, 5}; //non tested kp values
  float kd[3] = {4, 8, 4.4};

  //Finding the angles and velocities
  for (int i = 0; i < 3; i++) {

    errTheta[i] = errorFunc(curTheta[i], Thetaref[i]);
    errDTheta[i] = errorFunc(curDTheta[i], dThetaref[i]);
    errDDTheta[i] = ddThetaref[i] + (kp[i] * errTheta[i]) + (kd[i] * errDTheta[i]);
  }

  //computing the dynamics
  BLA::Matrix<3, 3> H = computeH(curTheta);
  BLA::Matrix<1, 3> C = computeC(curTheta, curDTheta);
  BLA::Matrix<1, 3> G = computeG(curTheta);

  for (int i = 0; i < 3; i++) {
    //finding the H with control system
    float Hi = errH(H(i, 0), H(i, 1), H(i, 2), errDDTheta); //H(currTheta)*(ddThetaref + kp*E + kd*dE)

    Q(0, i) = Hi + C(0, i) + G(0, i);
    //setPWM(DXL_ID[i+1], Qi);
  }

  return Q;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////




void setup() {
  // put your setup code here, to run once:
  Serial.begin(57600);
  while (!Serial); //Wait for serial port to be available

  //Start serial port for sending commands
  soft_serial.begin(57600);

}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.println("start");
  if (readInput(500) == true) {

    BLA::Matrix<1, 3> torque = PWMcalc(Thetaref,  dThetaref, ddThetaref);
    byte returnValue[8];
    int pwmValue[3];
    for(int i=0; i<3; i++){
      pwmValue[i] = torque(0, i);
      }
    
  returnValue[0]=startByte;
  for(int i=0; i<3; i++){
    returnValue[i*2+1]=highByte(pwmValue[i]);
    returnValue[i*2+2]=lowByte(pwmValue[i]);
    }
  crc.reset();
  crc.setPolynome(0x07);
  for (int i = 0; i < 7; i++)
  {
    crc.add(returnValue[i]);
  }
    returnValue[7]=crc.getCRC();
    for(int i = 0; i < 8; i++){
    soft_serial.write(returnValue[i]);
    }
  }
  
}