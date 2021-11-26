#include <SoftwareSerial.h>
#include <BasicLinearAlgebra.h>
#include <math.h>

SoftwareSerial soft_serial(32, 14); // RX/TX

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

      byte checksum = 0;
      for (int i = 3; i <= 32; i++) {
        checksum += recieverByte[i];
      }
      if (checksum == 0xFF) {
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



//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////// This shouldn't be here but for some reason i can't get it to compile /////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                                        ////

/*/////////////////////////////////////////////////////////////////////////////////////////////////
                    Holy shit guys don't put delays in this
  /////////////////////////////////////////////////////////////////////////////////////////////////*/

BLA::Matrix<3, 3> computeH(float theta[3]) {
  //Function for calculating the mass matrix

  //defining thetas
  float theta1 = (theta[0] - 190) * PI / 180, theta2 = (theta[1] - 180) * PI / 180, theta3 = (theta[2] - 180) * PI / 180;

  //defing the mass matrix
  float H11 = 0.157 * sin(2.0 * theta2 - 1.0 * theta1 + 2.0 * theta3) - 0.0042 * sin(theta1 + theta2 + theta3) - 0.0019 * cos(theta1 - 2.0 * theta2) - 0.0019 * cos(theta1 + 2.0 * theta2) - 0.00251 * cos(2.0 * theta1 + theta2) - 0.0014 * cos(2.0 * theta1 + theta3) - 0.00115 * cos(2.0 * theta2 + theta3) - 0.00421 * sin(theta1 - 1.0 * theta2) - 0.161 * sin(theta1 - 2.0 * theta2) + 0.161 * sin(theta1 + 2.0 * theta2) - 0.0019 * cos(theta1 + 2.0 * theta2 + 2.0 * theta3) + 0.0014 * cos(2.0 * theta2 - 2.0 * theta1 + theta3) + 0.0014 * cos(2.0 * theta1 + 2.0 * theta2 + theta3) + 0.00471 * sin(2.0 * theta2 - 1.0 * theta1 + theta3) + 0.157 * sin(theta1 + 2.0 * theta2 + 2.0 * theta3) - 0.208 * cos(2.0 * theta1) - 0.0392 * cos(2.0 * theta2) - 0.00421 * sin(theta1 + theta2) + 0.0025 * cos(theta2 - 2.0 * theta1 + theta3) - 0.0025 * cos(2.0 * theta1 + theta2 + theta3) + 0.0042 * sin(theta2 - 1.0 * theta1 + theta3) + 0.00471 * sin(theta1 + 2.0 * theta2 + theta3) - 0.00279 * cos(theta1) + 0.0124 * cos(theta3) + 0.00251 * cos(2.0 * theta1 - 1.0 * theta2) + 0.048 * cos(2.0 * theta1 - 2.0 * theta2) + 0.048 * cos(2.0 * theta1 + 2.0 * theta2) - 0.0014 * cos(2.0 * theta1 - 1.0 * theta3) - 0.0382 * cos(2.0 * theta2 + 2.0 * theta3) - 0.0019 * cos(2.0 * theta2 - 1.0 * theta1 + 2.0 * theta3) + 0.0467 * cos(2.0 * theta2 - 2.0 * theta1 + 2.0 * theta3) + 0.0467 * cos(2.0 * theta1 + 2.0 * theta2 + 2.0 * theta3) + 1.08;
  float H12 = 0.00326 * sin(theta1 + theta2 + theta3) - 0.00327 * sin(theta1 - 1.0 * theta2) + 0.00733 * sin(theta1 - 1.0 * theta3) - 0.00548 * cos(theta2 + theta3) + 0.00327 * sin(theta1 + theta2) + 0.00733 * sin(theta1 + theta3) + 0.00326 * sin(theta2 - 1.0 * theta1 + theta3) - 0.00549 * cos(theta2) + 1.11 * sin(theta1);
  float H13 = 0.00326 * sin(theta1 + theta2 + theta3) + 0.00366 * sin(theta1 - 1.0 * theta3) - 0.00548 * cos(theta2 + theta3) + 0.00366 * sin(theta1 + theta3) + 0.00326 * sin(theta2 - 1.0 * theta1 + theta3) + 0.546 * sin(theta1);

  float H21 = 0.00326 * sin(theta1 + theta2 + theta3) - 0.00327 * sin(theta1 - 1.0 * theta2) + 0.00733 * sin(theta1 - 1.0 * theta3) - 0.00548 * cos(theta2 + theta3) + 0.00327 * sin(theta1 + theta2) + 0.00733 * sin(theta1 + theta3) + 0.00326 * sin(theta2 - 1.0 * theta1 + theta3) - 0.00549 * cos(theta2) + 1.11 * sin(theta1);
  float H22 = 0.0191 * cos(theta3) + 1.44;
  float H23 = 0.00957 * cos(theta3) + 0.713;

  float H31 = 0.00326 * sin(theta1 + theta2 + theta3) + 0.00366 * sin(theta1 - 1.0 * theta3) - 0.00548 * cos(theta2 + theta3) + 0.00366 * sin(theta1 + theta3) + 0.00326 * sin(theta2 - 1.0 * theta1 + theta3) + 0.546 * sin(theta1);
  float H32 = 0.00957 * cos(theta3) + 0.713;
  float H33 = 0.713;
  return {H11, H12, H13,
          H21, H22, H23,
          H31, H32, H33};
}


BLA::Matrix<1, 3> computeC(float theta[3], float dtheta[3]) {
  //Function for calculating the Coriolis effect


  //defining thetas
  float theta1 = (theta[0] - 190) * PI / 180, theta2 = (theta[1] - 180) * PI / 180, theta3 = (theta[2] - 180) * PI / 180;
  //velocity
  float dtheta1 = dtheta[0] * PI / 180, dtheta2 = dtheta[1] * PI / 180, dtheta3 = dtheta[2] * PI / 180;

  //defining the coriolis effect vector
  float C1 = 0.0014 * pow(dtheta1, 2) * sin(theta1) + 0.00549 * pow(dtheta2, 2) * sin(theta2) + 0.208 * pow(dtheta1, 2) * sin(2.0 * theta1) - 0.0124 * dtheta1 * dtheta3 * sin(theta3) - 0.0959 * pow(dtheta1, 2) * cos(2.0 * theta2) * sin(2.0 * theta1) - 0.00113 * pow(dtheta1, 2) * sin(2.0 * theta1) * sin(2.0 * theta2) + 0.00502 * pow(dtheta1, 2) * cos(2.0 * theta1) * sin(theta2) + 0.0019 * pow(dtheta1, 2) * cos(2.0 * theta2) * sin(theta1) + 0.00281 * pow(dtheta1, 2) * sin(2.0 * theta1) * cos(theta3) - 0.161 * pow(dtheta1, 2) * sin(2.0 * theta2) * sin(theta1) + 0.0783 * dtheta1 * dtheta2 * sin(2.0 * theta2) - 0.00421 * pow(dtheta1, 2) * cos(theta1) * cos(theta2) + 0.00655 * pow(dtheta2, 2) * cos(theta1) * cos(theta2) + 0.00548 * pow(dtheta2, 2) * cos(theta2) * sin(theta3) + 0.00548 * pow(dtheta2, 2) * cos(theta3) * sin(theta2) + 0.00548 * pow(dtheta3, 2) * cos(theta2) * sin(theta3) + 0.00548 * pow(dtheta3, 2) * cos(theta3) * sin(theta2) - 0.00733 * pow(dtheta3, 2) * sin(theta1) * sin(theta3) - 0.157 * pow(dtheta1, 2) * cos(2.0 * theta2) * sin(2.0 * theta3) * sin(theta1) - 0.157 * pow(dtheta1, 2) * cos(2.0 * theta3) * sin(2.0 * theta2) * sin(theta1) + 0.00226 * dtheta1 * dtheta2 * cos(2.0 * theta1) * cos(2.0 * theta2) + 0.00281 * pow(dtheta1, 2) * sin(2.0 * theta1) * sin(2.0 * theta2) * sin(theta3) - 0.0019 * pow(dtheta1, 2) * sin(2.0 * theta2) * sin(2.0 * theta3) * sin(theta1) - 0.192 * dtheta1 * dtheta2 * cos(2.0 * theta1) * sin(2.0 * theta2) + 0.0763 * dtheta1 * dtheta2 * cos(2.0 * theta2) * sin(2.0 * theta3) + 0.0763 * dtheta1 * dtheta2 * cos(2.0 * theta3) * sin(2.0 * theta2) + 0.0763 * dtheta1 * dtheta3 * cos(2.0 * theta2) * sin(2.0 * theta3) + 0.0763 * dtheta1 * dtheta3 * cos(2.0 * theta3) * sin(2.0 * theta2) + 0.005 * pow(dtheta1, 2) * cos(2.0 * theta1) * cos(theta2) * sin(theta3) + 0.005 * pow(dtheta1, 2) * cos(2.0 * theta1) * cos(theta3) * sin(theta2) - 0.00471 * pow(dtheta1, 2) * cos(2.0 * theta2) * sin(theta1) * sin(theta3) - 0.00471 * pow(dtheta1, 2) * sin(2.0 * theta2) * cos(theta3) * sin(theta1) + 0.644 * dtheta1 * dtheta2 * cos(2.0 * theta2) * cos(theta1) + 0.00502 * dtheta1 * dtheta2 * sin(2.0 * theta1) * cos(theta2) + 0.00759 * dtheta1 * dtheta2 * sin(2.0 * theta2) * cos(theta1) + 0.00229 * dtheta1 * dtheta2 * cos(2.0 * theta2) * sin(theta3) + 0.00229 * dtheta1 * dtheta2 * sin(2.0 * theta2) * cos(theta3) + 0.00281 * dtheta1 * dtheta3 * cos(2.0 * theta1) * sin(theta3) + 0.00115 * dtheta1 * dtheta3 * cos(2.0 * theta2) * sin(theta3) + 0.00115 * dtheta1 * dtheta3 * sin(2.0 * theta2) * cos(theta3) - 0.0935 * pow(dtheta1, 2) * cos(2.0 * theta2) * cos(2.0 * theta3) * sin(2.0 * theta1) - 0.00113 * pow(dtheta1, 2) * cos(2.0 * theta2) * sin(2.0 * theta1) * sin(2.0 * theta3) - 0.00113 * pow(dtheta1, 2) * cos(2.0 * theta3) * sin(2.0 * theta1) * sin(2.0 * theta2) - 0.0042 * pow(dtheta1, 2) * cos(theta1) * cos(theta2) * cos(theta3) + 0.00653 * pow(dtheta2, 2) * cos(theta1) * cos(theta2) * cos(theta3) + 0.00653 * pow(dtheta3, 2) * cos(theta1) * cos(theta2) * cos(theta3) + 0.0935 * pow(dtheta1, 2) * sin(2.0 * theta1) * sin(2.0 * theta2) * sin(2.0 * theta3) + 0.0042 * pow(dtheta1, 2) * cos(theta1) * sin(theta2) * sin(theta3) - 0.00653 * pow(dtheta2, 2) * cos(theta1) * sin(theta2) * sin(theta3) - 0.00653 * pow(dtheta3, 2) * cos(theta1) * sin(theta2) * sin(theta3) + 0.011 * dtheta2 * dtheta3 * cos(theta2) * sin(theta3) + 0.011 * dtheta2 * dtheta3 * cos(theta3) * sin(theta2) + 0.00842 * dtheta1 * dtheta2 * sin(theta1) * sin(theta2) - 0.0147 * dtheta2 * dtheta3 * sin(theta1) * sin(theta3) + 0.0019 * pow(dtheta1, 2) * cos(2.0 * theta2) * cos(2.0 * theta3) * sin(theta1) - 0.00281 * pow(dtheta1, 2) * cos(2.0 * theta2) * sin(2.0 * theta1) * cos(theta3) - 0.628 * dtheta1 * dtheta2 * sin(2.0 * theta2) * sin(2.0 * theta3) * cos(theta1) - 0.628 * dtheta1 * dtheta3 * sin(2.0 * theta2) * sin(2.0 * theta3) * cos(theta1) + 0.0188 * dtheta1 * dtheta2 * cos(2.0 * theta2) * cos(theta1) * cos(theta3) + 0.00942 * dtheta1 * dtheta3 * cos(2.0 * theta2) * cos(theta1) * cos(theta3) + 0.005 * dtheta1 * dtheta2 * sin(2.0 * theta1) * cos(theta2) * cos(theta3) + 0.005 * dtheta1 * dtheta3 * sin(2.0 * theta1) * cos(theta2) * cos(theta3) - 0.0188 * dtheta1 * dtheta2 * sin(2.0 * theta2) * cos(theta1) * sin(theta3) - 0.00942 * dtheta1 * dtheta3 * sin(2.0 * theta2) * cos(theta1) * sin(theta3) - 0.005 * dtheta1 * dtheta2 * sin(2.0 * theta1) * sin(theta2) * sin(theta3) - 0.005 * dtheta1 * dtheta3 * sin(2.0 * theta1) * sin(theta2) * sin(theta3) + 0.00227 * dtheta1 * dtheta2 * cos(2.0 * theta1) * cos(2.0 * theta2) * cos(2.0 * theta3) + 0.00227 * dtheta1 * dtheta3 * cos(2.0 * theta1) * cos(2.0 * theta2) * cos(2.0 * theta3) - 0.187 * dtheta1 * dtheta2 * cos(2.0 * theta1) * cos(2.0 * theta2) * sin(2.0 * theta3) - 0.187 * dtheta1 * dtheta2 * cos(2.0 * theta1) * cos(2.0 * theta3) * sin(2.0 * theta2) - 0.187 * dtheta1 * dtheta3 * cos(2.0 * theta1) * cos(2.0 * theta2) * sin(2.0 * theta3) - 0.187 * dtheta1 * dtheta3 * cos(2.0 * theta1) * cos(2.0 * theta3) * sin(2.0 * theta2) - 0.00227 * dtheta1 * dtheta2 * cos(2.0 * theta1) * sin(2.0 * theta2) * sin(2.0 * theta3) - 0.00227 * dtheta1 * dtheta3 * cos(2.0 * theta1) * sin(2.0 * theta2) * sin(2.0 * theta3) + 0.0131 * dtheta2 * dtheta3 * cos(theta1) * cos(theta2) * cos(theta3) + 0.00839 * dtheta1 * dtheta2 * cos(theta2) * sin(theta1) * sin(theta3) + 0.00839 * dtheta1 * dtheta2 * cos(theta3) * sin(theta1) * sin(theta2) + 0.00839 * dtheta1 * dtheta3 * cos(theta2) * sin(theta1) * sin(theta3) + 0.00839 * dtheta1 * dtheta3 * cos(theta3) * sin(theta1) * sin(theta2) - 0.0131 * dtheta2 * dtheta3 * cos(theta1) * sin(theta2) * sin(theta3) + 0.628 * dtheta1 * dtheta2 * cos(2.0 * theta2) * cos(2.0 * theta3) * cos(theta1) + 0.628 * dtheta1 * dtheta3 * cos(2.0 * theta2) * cos(2.0 * theta3) * cos(theta1) - 0.00561 * dtheta1 * dtheta2 * cos(2.0 * theta1) * cos(2.0 * theta2) * sin(theta3) - 0.00561 * dtheta1 * dtheta2 * cos(2.0 * theta1) * sin(2.0 * theta2) * cos(theta3) + 0.00762 * dtheta1 * dtheta2 * cos(2.0 * theta2) * sin(2.0 * theta3) * cos(theta1) + 0.00762 * dtheta1 * dtheta2 * cos(2.0 * theta3) * sin(2.0 * theta2) * cos(theta1) - 0.00281 * dtheta1 * dtheta3 * cos(2.0 * theta1) * cos(2.0 * theta2) * sin(theta3) - 0.00281 * dtheta1 * dtheta3 * cos(2.0 * theta1) * sin(2.0 * theta2) * cos(theta3) + 0.00762 * dtheta1 * dtheta3 * cos(2.0 * theta2) * sin(2.0 * theta3) * cos(theta1) + 0.00762 * dtheta1 * dtheta3 * cos(2.0 * theta3) * sin(2.0 * theta2) * cos(theta1);
  float C2 = 1.11 * pow(dtheta1, 2) * cos(theta1) - 0.0392 * pow(dtheta1, 2) * sin(2.0 * theta2) - 0.00957 * pow(dtheta3, 2) * sin(theta3) - 0.0191 * dtheta2 * dtheta3 * sin(theta3) - 0.00113 * pow(dtheta1, 2) * cos(2.0 * theta1) * cos(2.0 * theta2) + 0.0959 * pow(dtheta1, 2) * cos(2.0 * theta1) * sin(2.0 * theta2) - 0.0382 * pow(dtheta1, 2) * cos(2.0 * theta2) * sin(2.0 * theta3) - 0.0382 * pow(dtheta1, 2) * cos(2.0 * theta3) * sin(2.0 * theta2) - 0.322 * pow(dtheta1, 2) * cos(2.0 * theta2) * cos(theta1) - 0.00251 * pow(dtheta1, 2) * sin(2.0 * theta1) * cos(theta2) - 0.00379 * pow(dtheta1, 2) * sin(2.0 * theta2) * cos(theta1) - 0.00115 * pow(dtheta1, 2) * cos(2.0 * theta2) * sin(theta3) - 0.00115 * pow(dtheta1, 2) * sin(2.0 * theta2) * cos(theta3) + 0.0147 * pow(dtheta1, 2) * cos(theta1) * cos(theta3) - 0.0108 * pow(dtheta1, 2) * sin(theta1) * sin(theta2) + 0.314 * pow(dtheta1, 2) * sin(2.0 * theta2) * sin(2.0 * theta3) * cos(theta1) - 0.00942 * pow(dtheta1, 2) * cos(2.0 * theta2) * cos(theta1) * cos(theta3) - 0.0025 * pow(dtheta1, 2) * sin(2.0 * theta1) * cos(theta2) * cos(theta3) + 0.00942 * pow(dtheta1, 2) * sin(2.0 * theta2) * cos(theta1) * sin(theta3) + 0.0025 * pow(dtheta1, 2) * sin(2.0 * theta1) * sin(theta2) * sin(theta3) - 0.00113 * pow(dtheta1, 2) * cos(2.0 * theta1) * cos(2.0 * theta2) * cos(2.0 * theta3) + 0.0935 * pow(dtheta1, 2) * cos(2.0 * theta1) * cos(2.0 * theta2) * sin(2.0 * theta3) + 0.0935 * pow(dtheta1, 2) * cos(2.0 * theta1) * cos(2.0 * theta3) * sin(2.0 * theta2) + 0.00113 * pow(dtheta1, 2) * cos(2.0 * theta1) * sin(2.0 * theta2) * sin(2.0 * theta3) - 0.0107 * pow(dtheta1, 2) * cos(theta2) * sin(theta1) * sin(theta3) - 0.0107 * pow(dtheta1, 2) * cos(theta3) * sin(theta1) * sin(theta2) - 0.314 * pow(dtheta1, 2) * cos(2.0 * theta2) * cos(2.0 * theta3) * cos(theta1) - 0.0147 * dtheta1 * dtheta3 * sin(theta1) * sin(theta3) + 0.00281 * pow(dtheta1, 2) * cos(2.0 * theta1) * cos(2.0 * theta2) * sin(theta3) + 0.00281 * pow(dtheta1, 2) * cos(2.0 * theta1) * sin(2.0 * theta2) * cos(theta3) - 0.00381 * pow(dtheta1, 2) * cos(2.0 * theta2) * sin(2.0 * theta3) * cos(theta1) - 0.00381 * pow(dtheta1, 2) * cos(2.0 * theta3) * sin(2.0 * theta2) * cos(theta1);
  float C3 = 0.00619 * pow(dtheta1, 2) * sin(theta3) + 0.00957 * pow(dtheta2, 2) * sin(theta3) + 0.546 * pow(dtheta1, 2) * cos(theta1) - 0.0382 * pow(dtheta1, 2) * cos(2.0 * theta2) * sin(2.0 * theta3) - 0.0382 * pow(dtheta1, 2) * cos(2.0 * theta3) * sin(2.0 * theta2) - 0.0014 * pow(dtheta1, 2) * cos(2.0 * theta1) * sin(theta3) + 0.00733 * pow(dtheta1, 2) * cos(theta1) * cos(theta3) + 0.314 * pow(dtheta1, 2) * sin(2.0 * theta2) * sin(2.0 * theta3) * cos(theta1) - 0.00471 * pow(dtheta1, 2) * cos(2.0 * theta2) * cos(theta1) * cos(theta3) - 0.0025 * pow(dtheta1, 2) * sin(2.0 * theta1) * cos(theta2) * cos(theta3) + 0.00471 * pow(dtheta1, 2) * sin(2.0 * theta2) * cos(theta1) * sin(theta3) + 0.0025 * pow(dtheta1, 2) * sin(2.0 * theta1) * sin(theta2) * sin(theta3) - 0.00113 * pow(dtheta1, 2) * cos(2.0 * theta1) * cos(2.0 * theta2) * cos(2.0 * theta3) + 0.0935 * pow(dtheta1, 2) * cos(2.0 * theta1) * cos(2.0 * theta2) * sin(2.0 * theta3) + 0.0935 * pow(dtheta1, 2) * cos(2.0 * theta1) * cos(2.0 * theta3) * sin(2.0 * theta2) + 0.00113 * pow(dtheta1, 2) * cos(2.0 * theta1) * sin(2.0 * theta2) * sin(2.0 * theta3) - 0.0107 * pow(dtheta1, 2) * cos(theta2) * sin(theta1) * sin(theta3) - 0.0107 * pow(dtheta1, 2) * cos(theta3) * sin(theta1) * sin(theta2) - 0.314 * pow(dtheta1, 2) * cos(2.0 * theta2) * cos(2.0 * theta3) * cos(theta1) + 0.0147 * dtheta1 * dtheta2 * sin(theta1) * sin(theta3) + 0.0014 * pow(dtheta1, 2) * cos(2.0 * theta1) * cos(2.0 * theta2) * sin(theta3) + 0.0014 * pow(dtheta1, 2) * cos(2.0 * theta1) * sin(2.0 * theta2) * cos(theta3) - 0.00381 * pow(dtheta1, 2) * cos(2.0 * theta2) * sin(2.0 * theta3) * cos(theta1) - 0.00381 * pow(dtheta1, 2) * cos(2.0 * theta3) * sin(2.0 * theta2) * cos(theta1);

  return {C1,
          C2,
          C3};
}

BLA::Matrix<1, 3> computeG(float theta[3]) {
  //Function for calculating the Gravity effect


  //defining thetas
  float theta1 = (theta[0] - 190) * PI / 180, theta2 = (theta[1] - 180) * PI / 180, theta3 = (theta[2] - 180) * PI / 180;
  //calcs
  float G1 = - 0.0014 * cos(theta1) - 0.832 * sin(theta1) * sin(theta2) - 0.327 * cos(theta2) * sin(theta1) * sin(theta3) - 0.327 * cos(theta3) * sin(theta1) * sin(theta2);
  float G2 = 0.698 * sin(theta2) + 0.832 * cos(theta1) * cos(theta2) + 0.275 * cos(theta2) * sin(theta3) + 0.275 * cos(theta3) * sin(theta2) - 0.327 * cos(theta1) * sin(theta2) * sin(theta3) + 0.327 * cos(theta1) * cos(theta2) * cos(theta3);
  float G3 = 0.275 * cos(theta2) * sin(theta3) + 0.275 * cos(theta3) * sin(theta2) - 0.327 * cos(theta1) * sin(theta2) * sin(theta3) + 0.327 * cos(theta1) * cos(theta2) * cos(theta3);

  return { -G1,
           -G2,
           -G3};
}



float errorFunc(float measured, float desired) { //error function
  return desired - measured;
}

void updateRef() { //Function for updating the references (should be set up with trajectory later)
}



float errH(float H1, float H2, float H3, float ddtheta[3]) {
  //acceleration
  float ddtheta1 = ddtheta[0] * PI / 180, ddtheta2 = ddtheta[1] * PI / 180, ddtheta3 = ddtheta[2] * PI / 180;

  return H1 * ddtheta1 + H2 * ddtheta2 + H3 * ddtheta3;
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

////                                                                                                                      ////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////// This shouldn't be here but for some reason i can't get it to compile /////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////





void setup() {
  // put your setup code here, to run once:
  Serial.begin(57600);
  while (!Serial); //Wait for serial port to be available

  //Start serial port for sending commands
  soft_serial.begin(57600);

}

void loop() {
  // put your main code here, to run repeatedly:
  if (readInput(500) == true) {

    BLA::Matrix<1, 3> torque = PWMcalc(Thetaref,  dThetaref, ddThetaref);
    int pwmValue1 = torque(0, 0);
    int pwmValue2 = torque(0, 1);
    int pwmValue3 = torque(0, 2);

    soft_serial.write(startByte);
    soft_serial.write(highByte(pwmValue1));
    soft_serial.write(lowByte(pwmValue1));
    soft_serial.write(highByte(pwmValue2));
    soft_serial.write(lowByte(pwmValue2));
    soft_serial.write(highByte(pwmValue3));
    soft_serial.write(lowByte(pwmValue3));
  }
}
