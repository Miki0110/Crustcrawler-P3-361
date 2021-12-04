/*/////////////////////////////////////////////////////////////////////////////////////////////////
                    Holy shit guys don't put delays in this
/////////////////////////////////////////////////////////////////////////////////////////////////*/
int16_t PWMcalc(int id, float torque, float velocity){
  float c1;
  float c2;
  
 switch(id){ //there are different c values for each motor, plus the values change depending on the direction
  case 2:                     // For the MX-106 motor
    if(velocity > 0){
    c1=138.81;
    c2=6.1305;
  }else if(velocity = 0){
    c1=(138.81+40.4)/2;
    c2=6.1305;    
    }else{
    c1=40.4;
    c2=6.1305;
      }
  break;
  default:                  // For the MX-64 motor
    if(velocity > 0){
    c1=253.75;
    c2=6.1305;
  }else if(velocity = 0){
    c1=(253.75+80.9)/2;
    c2=6.1305;    
    }else{
    c1=80.9;
    c2=6.1305;
      }
  break;
 }
 return torque*c1+velocity*c2;
}


BLA::Matrix<3,3> computeH(float theta[3]){
//Function for calculating the mass matrix

//defining thetas
float theta1=(theta[0]-190)*PI/180, theta2=(theta[1]-180)*PI/180, theta3=(theta[2]-180)*PI/180;

//defing the mass matrix
float H11=0.0017*sin(2.0*theta3 + 2.0*theta2 - 1.0*theta1) - 8.7e-6*sin(theta3 + theta2 + theta1) + 5.6e-6*cos(- 2.0*theta2 + theta1) + 5.6e-6*cos(2.0*theta2 + theta1) - 1.32e-5*cos(theta2 + 2.0*theta1) - 0.0014*cos(theta3 + 2.0*theta1) - 0.0011*cos(theta3 + 2.0*theta2) - 2.21e-5*sin(- 1.0*theta2 + theta1) - 0.0057*sin(- 2.0*theta2 + theta1) + 0.0057*sin(2.0*theta2 + theta1) - 1.9e-6*cos(2.0*theta3 + 2.0*theta2 + theta1) + 0.0014*cos(theta3 + 2.0*theta2 - 2.0*theta1) + 0.0014*cos(theta3 + 2.0*theta2 + 2.0*theta1) + 0.0047*sin(theta3 + 2.0*theta2 - 1.0*theta1) + 0.0017*sin(2.0*theta3 + 2.0*theta2 + theta1) - 0.0044*cos(2.0*theta1) - 0.0014*cos(2.0*theta2) + 1.4e-6*sin(2.0*theta2) - 2.21e-5*sin(theta2 + theta1) + 5.2e-6*cos(theta3 + theta2 - 2.0*theta1) - 5.2e-6*cos(theta3 + theta2 + 2.0*theta1) + 8.7e-6*sin(theta3 + theta2 - 1.0*theta1) + 0.0047*sin(theta3 + 2.0*theta2 + theta1) + 0.0124*cos(theta3) + 1.32e-5*cos(- 1.0*theta2 + 2.0*theta1) + 0.0017*cos(- 2.0*theta2 + 2.0*theta1) + 0.0017*cos(2.0*theta2 + 2.0*theta1) - 0.0014*cos(- 1.0*theta3 + 2.0*theta1) - 0.0004025*cos(2.0*theta3 + 2.0*theta2) + 1.7e-6*sin(- 2.0*theta2 + 2.0*theta1) - 1.7e-6*sin(2.0*theta2 + 2.0*theta1) - 1.9e-6*cos(2.0*theta3 + 2.0*theta2 - 1.0*theta1) + 0.0004931*cos(2.0*theta3 + 2.0*theta2 - 2.0*theta1) + 0.0004931*cos(2.0*theta3 + 2.0*theta2 + 2.0*theta1) + 0.0198;
float H12=6.8e-6*sin(theta3 + theta2 + theta1) - 1.72e-5*sin(- 1.0*theta2 + theta1) + 0.0073*sin(- 1.0*theta3 + theta1) - 1.13e-5*cos(theta3 + theta2) + 1.72e-5*sin(theta2 + theta1) + 0.0073*sin(theta3 + theta1) + 6.8e-6*sin(theta3 + theta2 - 1.0*theta1) - 2.88e-5*cos(theta2) + 0.0231*sin(theta1);
float H13=6.8e-6*sin(theta3 + theta2 + theta1) + 0.0037*sin(- 1.0*theta3 + theta1) - 1.13e-5*cos(theta3 + theta2) + 0.0037*sin(theta3 + theta1) + 6.8e-6*sin(theta3 + theta2 - 1.0*theta1) + 0.0052*sin(theta1);
float H21=6.8e-6*sin(theta3 + theta2 + theta1) - 1.72e-5*sin(- 1.0*theta2 + theta1) + 0.0073*sin(- 1.0*theta3 + theta1) - 1.13e-5*cos(theta3 + theta2) + 1.72e-5*sin(theta2 + theta1) + 0.0073*sin(theta3 + theta1) + 6.8e-6*sin(theta3 + theta2 - 1.0*theta1) - 2.88e-5*cos(theta2) + 0.0231*sin(theta1);
float H22=0.0191*cos(theta3) + 0.0302;
float H23=0.00957*cos(theta3) + 0.0068;
float H31=6.8e-6*sin(theta3 + theta2 + theta1) + 0.0037*sin(- 1.0*theta3 + theta1) - 1.13e-5*cos(theta3 + theta2) + 0.0037*sin(theta3 + theta1) + 6.8e-6*sin(theta3 + theta2 - 1.0*theta1) + 0.0052*sin(theta1);
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


float errH(float H1, float H2, float H3, float ddtheta[3]){
  //acceleration
  float ddtheta1=ddtheta[0]*PI/180,ddtheta2=ddtheta[1]*PI/180,ddtheta3=ddtheta[2]*PI/180;

  return H1*ddtheta1+H2*ddtheta2+H3*ddtheta3;
  }

  
void torqueCalc(float Thetaref[3], float dThetaref[3], float ddThetaref[3], float curTheta[3], float curDTheta[3]){
//BLA::Matrix<1, 3> torqueCalc() {

  //due to the difference between the used 0 values and the motors values an offset is input
  Thetaref[0] = Thetaref[0] + 191.6, Thetaref[1] = Thetaref[1] + 180, Thetaref[2] = Thetaref[2] + 180;

  //initializing variables
  float errTheta[3], errDTheta[3], errDDTheta[3];

  float kp[3] = {64, 196, 784};
  float kd[3] = {16, 28, 56};

  //Finding the angles and velocities
  for (int i = 0; i < 3; i++) {

    errTheta[i] = errorFunc(curTheta[i], Thetaref[i]);
    errDTheta[i] = errorFunc(curDTheta[i], dThetaref[i]);
    errDDTheta[i] = ddThetaref[i] + (kp[i] * errTheta[i]) + (kd[i] * errDTheta[i]);
  }
  Serial.print("Error Theta 1: ");
  Serial.println(errTheta[0]);

  
  Serial.print("Error Theta 2: ");
  Serial.println(errTheta[1]);

  
  Serial.print("Error Theta 3: ");
  Serial.println(errTheta[2]);

  //computing the dynamics
  BLA::Matrix<3, 3> H = computeH(curTheta);
  BLA::Matrix<1, 3> C = computeC(curTheta, curDTheta);
  BLA::Matrix<1, 3> G = computeG(curTheta);

  for (int i = 0; i < 3; i++) {
    //finding the H with control system
    float Hi = errH(H(i, 0), H(i, 1), H(i, 2), errDDTheta); //H(currTheta)*(ddThetaref + kp*E + kd*dE)

    Q(0, i) = (Hi + C(0, i) + G(0, i));
    //setPWM(DXL_ID[i+1], Qi);
  }

 
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////
