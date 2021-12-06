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
float theta1=(theta[0]-180)*PI/180, theta2=(theta[1]-180)*PI/180, theta3=(theta[2]-180)*PI/180;

//defing the mass matrix
float H11=0.0197*sin(2*theta3 + 2*theta2 - 1*theta1) + 0.0037*sin(2*theta3 + 2*theta2 - 2*theta1) + 0.0037*sin(2*theta3 + 2*theta2 + 2*theta1) - 9.9600e-05*cos(theta3 + theta2 + theta1) - 3.2120e-04*sin(theta3 + theta2 + theta1) + 1.1700e-05*cos(- 2*theta2 + theta1) + 1.1700e-05*cos(2*theta2 + theta1) - 5.3000e-06*cos(theta2 + 2*theta1) - 0.0014*cos(theta3 + 2*theta1) - 0.0011*cos(theta3 + 2*theta2) - 8.9000e-06*sin(- 1*theta2 + theta1) - 0.0057*sin(- 2*theta2 + theta1) + 0.0057*sin(2*theta2 + theta1) + 2.1000e-06*sin(theta3 + 2*theta1) + 1.7000e-06*sin(theta3 + 2*theta2) + 7.1000e-06*cos(theta3 + 2*theta2 - 1*theta1) - 0.0124*cos(2*theta3 + 2*theta2 + theta1) + 0.0014*cos(theta3 + 2*theta2 - 2*theta1) + 0.0014*cos(theta3 + 2*theta2 + 2*theta1) + 0.0047*sin(theta3 + 2*theta2 - 1*theta1) + 0.0197*sin(2*theta3 + 2*theta2 + theta1) - 2.1000e-06*sin(theta3 + 2*theta2 - 2*theta1) - 2.1000e-06*sin(theta3 + 2*theta2 + 2*theta1) - 0.0174*cos(2*theta1) - 0.0014*cos(2*theta2) + 2.8000e-06*sin(2*theta2) - 8.9000e-06*sin(theta2 + theta1) + 9.9600e-05*cos(theta3 + theta2 - 1*theta1) + 7.1000e-06*cos(theta3 + 2*theta2 + theta1) + 1.9140e-04*cos(theta3 + theta2 - 2*theta1) - 1.9140e-04*cos(theta3 + theta2 + 2*theta1) + 3.2120e-04*sin(theta3 + theta2 - 1*theta1) + 0.0047*sin(theta3 + 2*theta2 + theta1) - 5.9300e-05*sin(theta3 + theta2 - 2*theta1) + 5.9300e-05*sin(theta3 + theta2 + 2*theta1) + 0.0124*cos(theta3) - 1.8600e-05*sin(theta3) + 5.3000e-06*cos(- 1*theta2 + 2*theta1) + 0.0017*cos(- 2*theta2 + 2*theta1) + 0.0017*cos(2*theta2 + 2*theta1) - 0.0014*cos(- 1*theta3 + 2*theta1) - 0.0048*cos(2*theta3 + 2*theta2) + 3.5000e-06*sin(- 2*theta2 + 2*theta1) - 3.5000e-06*sin(2*theta2 + 2*theta1) - 2.1000e-06*sin(- 1*theta3 + 2*theta1) - 0.0030*sin(2*theta3 + 2*theta2) - 0.0124*cos(2*theta3 + 2*theta2 - 1*theta1) + 0.0059*cos(2*theta3 + 2*theta2 - 2*theta1) + 0.0059*cos(2*theta3 + 2*theta2 + 2*theta1) + 0.0773;
float H12=7.7500e-05*cos(theta3 + theta2 + theta1) + 2.4990e-04*sin(theta3 + theta2 + theta1) - 1.1000e-05*cos(- 1*theta3 + theta1) - 6.9000e-06*sin(- 1*theta2 + theta1) + 0.0073*sin(- 1*theta3 + theta1) + 1.1000e-05*cos(theta3 + theta1) - 4.1940e-04*cos(theta3 + theta2) + 6.9000e-06*sin(theta2 + theta1) + 0.0073*sin(theta3 + theta1) + 1.3000e-04*sin(theta3 + theta2) + 7.7500e-05*cos(theta3 + theta2 - 1*theta1) + 2.4990e-04*sin(theta3 + theta2 - 1*theta1) - 1.1600e-05*cos(theta2) + 0.0911*sin(theta1);
float H13=7.7500e-05*cos(theta3 + theta2 + theta1) + 2.4990e-04*sin(theta3 + theta2 + theta1) - 5.5000e-06*cos(- 1*theta3 + theta1) + 0.0037*sin(- 1*theta3 + theta1) + 5.5000e-06*cos(theta3 + theta1) - 4.1940e-04*cos(theta3 + theta2) + 0.0037*sin(theta3 + theta1) + 1.3000e-04*sin(theta3 + theta2) + 7.7500e-05*cos(theta3 + theta2 - 1*theta1) + 2.4990e-04*sin(theta3 + theta2 - 1*theta1) + 0.0732*sin(theta1);
float H21=7.7500e-05*cos(theta3 + theta2 + theta1) + 2.4990e-04*sin(theta3 + theta2 + theta1) - 1.1000e-05*cos(- 1*theta3 + theta1) - 6.9000e-06*sin(- 1*theta2 + theta1) + 0.0073*sin(- 1*theta3 + theta1) + 1.1000e-05*cos(theta3 + theta1) - 4.1940e-04*cos(theta3 + theta2) + 6.9000e-06*sin(theta2 + theta1) + 0.0073*sin(theta3 + theta1) + 1.3000e-04*sin(theta3 + theta2) + 7.7500e-05*cos(theta3 + theta2 - 1*theta1) + 2.4990e-04*sin(theta3 + theta2 - 1*theta1) - 1.1600e-05*cos(theta2) + 0.0911*sin(theta1);
float H22=0.0191*cos(theta3) - 2.8700e-05*sin(theta3) + 0.1190;
float H23=0.0096*cos(theta3) - 1.4400e-05*sin(theta3) + 0.0956;
float H31=7.7500e-05*cos(theta3 + theta2 + theta1) + 2.4990e-04*sin(theta3 + theta2 + theta1) - 5.5000e-06*cos(- 1*theta3 + theta1) + 0.0037*sin(- 1*theta3 + theta1) + 5.5000e-06*cos(theta3 + theta1) - 4.1940e-04*cos(theta3 + theta2) + 0.0037*sin(theta3 + theta1) + 1.3000e-04*sin(theta3 + theta2) + 7.7500e-05*cos(theta3 + theta2 - 1*theta1) + 2.4990e-04*sin(theta3 + theta2 - 1*theta1) + 0.0732*sin(theta1);
float H32=0.0096*cos(theta3) - 1.4400e-05*sin(theta3) + 0.0956;
float H33=0.0956;
return {H11,H12,H13,
        H21,H22,H23,
        H31,H32,H33};
}


BLA::Matrix<1,3> computeC(float theta[3], float dtheta[3]){
//Function for calculating the Coriolis effect

  
//defining thetas
float theta1=(theta[0]-180)*PI/180, theta2=(theta[1]-180)*PI/180, theta3=(theta[2]-180)*PI/180;
//velocity
float dtheta1=dtheta[0]*PI/180, dtheta2=dtheta[1]*PI/180,dtheta3=dtheta[2]*PI/180;

//defining the coriolis effect vector
float C1=0.0174*pow(dtheta1,2)*sin(2*theta1) - 0.0124*dtheta1*dtheta3*sin(theta3) - 0.0034*pow(dtheta1,2)*cos(2*theta2)*sin(2*theta1) + 0.0028*pow(dtheta1,2)*sin(2*theta1)*cos(theta3) - 0.0057*pow(dtheta1,2)*sin(2*theta2)*sin(theta1) + 0.0028*dtheta1*dtheta2*sin(2*theta2) - 0.0073*pow(dtheta3,2)*sin(theta1)*sin(theta3) - 0.0197*pow(dtheta1,2)*cos(2*theta2)*sin(2*theta3)*sin(theta1) - 0.0197*pow(dtheta1,2)*cos(2*theta3)*sin(2*theta2)*sin(theta1) - 0.0060*dtheta1*dtheta2*cos(2*theta2)*cos(2*theta3) - 0.0060*dtheta1*dtheta3*cos(2*theta2)*cos(2*theta3) + 0.0028*pow(dtheta1,2)*sin(2*theta1)*sin(2*theta2)*sin(theta3) - 0.0124*pow(dtheta1,2)*sin(2*theta2)*sin(2*theta3)*sin(theta1) - 0.0068*dtheta1*dtheta2*cos(2*theta1)*sin(2*theta2) + 0.0096*dtheta1*dtheta2*cos(2*theta2)*sin(2*theta3) + 0.0096*dtheta1*dtheta2*cos(2*theta3)*sin(2*theta2) + 0.0096*dtheta1*dtheta3*cos(2*theta2)*sin(2*theta3) + 0.0096*dtheta1*dtheta3*cos(2*theta3)*sin(2*theta2) + 0.0060*dtheta1*dtheta2*sin(2*theta2)*sin(2*theta3) + 0.0060*dtheta1*dtheta3*sin(2*theta2)*sin(2*theta3) - 0.0047*pow(dtheta1,2)*cos(2*theta2)*sin(theta1)*sin(theta3) - 0.0047*pow(dtheta1,2)*sin(2*theta2)*cos(theta3)*sin(theta1) + 0.0230*dtheta1*dtheta2*cos(2*theta2)*cos(theta1) + 0.0023*dtheta1*dtheta2*cos(2*theta2)*sin(theta3) + 0.0023*dtheta1*dtheta2*sin(2*theta2)*cos(theta3) + 0.0028*dtheta1*dtheta3*cos(2*theta1)*sin(theta3) + 0.0011*dtheta1*dtheta3*cos(2*theta2)*sin(theta3) + 0.0011*dtheta1*dtheta3*sin(2*theta2)*cos(theta3) - 0.0117*pow(dtheta1,2)*cos(2*theta2)*cos(2*theta3)*sin(2*theta1) - 0.0074*pow(dtheta1,2)*cos(2*theta2)*sin(2*theta1)*sin(2*theta3) - 0.0074*pow(dtheta1,2)*cos(2*theta3)*sin(2*theta1)*sin(2*theta2) + 0.0117*pow(dtheta1,2)*sin(2*theta1)*sin(2*theta2)*sin(2*theta3) - 0.0147*dtheta2*dtheta3*sin(theta1)*sin(theta3) + 0.0124*pow(dtheta1,2)*cos(2*theta2)*cos(2*theta3)*sin(theta1) - 0.0028*pow(dtheta1,2)*cos(2*theta2)*sin(2*theta1)*cos(theta3) - 0.0787*dtheta1*dtheta2*sin(2*theta2)*sin(2*theta3)*cos(theta1) - 0.0787*dtheta1*dtheta3*sin(2*theta2)*sin(2*theta3)*cos(theta1) + 0.0188*dtheta1*dtheta2*cos(2*theta2)*cos(theta1)*cos(theta3) + 0.0094*dtheta1*dtheta3*cos(2*theta2)*cos(theta1)*cos(theta3) - 0.0188*dtheta1*dtheta2*sin(2*theta2)*cos(theta1)*sin(theta3) - 0.0094*dtheta1*dtheta3*sin(2*theta2)*cos(theta1)*sin(theta3) + 0.0147*dtheta1*dtheta2*cos(2*theta1)*cos(2*theta2)*cos(2*theta3) + 0.0147*dtheta1*dtheta3*cos(2*theta1)*cos(2*theta2)*cos(2*theta3) - 0.0235*dtheta1*dtheta2*cos(2*theta1)*cos(2*theta2)*sin(2*theta3) - 0.0235*dtheta1*dtheta2*cos(2*theta1)*cos(2*theta3)*sin(2*theta2) - 0.0235*dtheta1*dtheta3*cos(2*theta1)*cos(2*theta2)*sin(2*theta3) - 0.0235*dtheta1*dtheta3*cos(2*theta1)*cos(2*theta3)*sin(2*theta2) - 0.0147*dtheta1*dtheta2*cos(2*theta1)*sin(2*theta2)*sin(2*theta3) - 0.0147*dtheta1*dtheta3*cos(2*theta1)*sin(2*theta2)*sin(2*theta3) + 0.0010*dtheta2*dtheta3*cos(theta1)*cos(theta2)*cos(theta3) - 0.0010*dtheta2*dtheta3*cos(theta1)*sin(theta2)*sin(theta3) + 0.0787*dtheta1*dtheta2*cos(2*theta2)*cos(2*theta3)*cos(theta1) + 0.0787*dtheta1*dtheta3*cos(2*theta2)*cos(2*theta3)*cos(theta1) - 0.0056*dtheta1*dtheta2*cos(2*theta1)*cos(2*theta2)*sin(theta3) - 0.0056*dtheta1*dtheta2*cos(2*theta1)*sin(2*theta2)*cos(theta3) + 0.0495*dtheta1*dtheta2*cos(2*theta2)*sin(2*theta3)*cos(theta1) + 0.0495*dtheta1*dtheta2*cos(2*theta3)*sin(2*theta2)*cos(theta1) - 0.0028*dtheta1*dtheta3*cos(2*theta1)*cos(2*theta2)*sin(theta3) - 0.0028*dtheta1*dtheta3*cos(2*theta1)*sin(2*theta2)*cos(theta3) + 0.0495*dtheta1*dtheta3*cos(2*theta2)*sin(2*theta3)*cos(theta1) + 0.0495*dtheta1*dtheta3*cos(2*theta3)*sin(2*theta2)*cos(theta1);
float C2=0.0911*pow(dtheta1,2)*cos(theta1) - 0.0014*pow(dtheta1,2)*sin(2*theta2) - 0.0096*pow(dtheta3,2)*sin(theta3) - 0.0191*dtheta2*dtheta3*sin(theta3) + 0.0030*pow(dtheta1,2)*cos(2*theta2)*cos(2*theta3) + 0.0034*pow(dtheta1,2)*cos(2*theta1)*sin(2*theta2) - 0.0048*pow(dtheta1,2)*cos(2*theta2)*sin(2*theta3) - 0.0048*pow(dtheta1,2)*cos(2*theta3)*sin(2*theta2) - 0.0030*pow(dtheta1,2)*sin(2*theta2)*sin(2*theta3) - 0.0115*pow(dtheta1,2)*cos(2*theta2)*cos(theta1) - 0.0011*pow(dtheta1,2)*cos(2*theta2)*sin(theta3) - 0.0011*pow(dtheta1,2)*sin(2*theta2)*cos(theta3) + 0.0147*pow(dtheta1,2)*cos(theta1)*cos(theta3) + 0.0394*pow(dtheta1,2)*sin(2*theta2)*sin(2*theta3)*cos(theta1) - 0.0094*pow(dtheta1,2)*cos(2*theta2)*cos(theta1)*cos(theta3) + 0.0094*pow(dtheta1,2)*sin(2*theta2)*cos(theta1)*sin(theta3) - 0.0074*pow(dtheta1,2)*cos(2*theta1)*cos(2*theta2)*cos(2*theta3) + 0.0117*pow(dtheta1,2)*cos(2*theta1)*cos(2*theta2)*sin(2*theta3) + 0.0117*pow(dtheta1,2)*cos(2*theta1)*cos(2*theta3)*sin(2*theta2) + 0.0074*pow(dtheta1,2)*cos(2*theta1)*sin(2*theta2)*sin(2*theta3) - 0.0394*pow(dtheta1,2)*cos(2*theta2)*cos(2*theta3)*cos(theta1) - 0.0147*dtheta1*dtheta3*sin(theta1)*sin(theta3) + 0.0028*pow(dtheta1,2)*cos(2*theta1)*cos(2*theta2)*sin(theta3) + 0.0028*pow(dtheta1,2)*cos(2*theta1)*sin(2*theta2)*cos(theta3) - 0.0247*pow(dtheta1,2)*cos(2*theta2)*sin(2*theta3)*cos(theta1) - 0.0247*pow(dtheta1,2)*cos(2*theta3)*sin(2*theta2)*cos(theta1);
float C3=0.0062*pow(dtheta1,2)*sin(theta3) + 0.0096*pow(dtheta2,2)*sin(theta3) + 0.0732*pow(dtheta1,2)*cos(theta1) + 0.0030*pow(dtheta1,2)*cos(2*theta2)*cos(2*theta3) - 0.0048*pow(dtheta1,2)*cos(2*theta2)*sin(2*theta3) - 0.0048*pow(dtheta1,2)*cos(2*theta3)*sin(2*theta2) - 0.0030*pow(dtheta1,2)*sin(2*theta2)*sin(2*theta3) - 0.0014*pow(dtheta1,2)*cos(2*theta1)*sin(theta3) + 0.0073*pow(dtheta1,2)*cos(theta1)*cos(theta3) + 0.0394*pow(dtheta1,2)*sin(2*theta2)*sin(2*theta3)*cos(theta1) - 0.0047*pow(dtheta1,2)*cos(2*theta2)*cos(theta1)*cos(theta3) + 0.0047*pow(dtheta1,2)*sin(2*theta2)*cos(theta1)*sin(theta3) - 0.0074*pow(dtheta1,2)*cos(2*theta1)*cos(2*theta2)*cos(2*theta3) + 0.0117*pow(dtheta1,2)*cos(2*theta1)*cos(2*theta2)*sin(2*theta3) + 0.0117*pow(dtheta1,2)*cos(2*theta1)*cos(2*theta3)*sin(2*theta2) + 0.0074*pow(dtheta1,2)*cos(2*theta1)*sin(2*theta2)*sin(2*theta3) - 0.0394*pow(dtheta1,2)*cos(2*theta2)*cos(2*theta3)*cos(theta1) + 0.0147*dtheta1*dtheta2*sin(theta1)*sin(theta3) + 0.0014*pow(dtheta1,2)*cos(2*theta1)*cos(2*theta2)*sin(theta3) + 0.0014*pow(dtheta1,2)*cos(2*theta1)*sin(2*theta2)*cos(theta3) - 0.0247*pow(dtheta1,2)*cos(2*theta2)*sin(2*theta3)*cos(theta1) - 0.0247*pow(dtheta1,2)*cos(2*theta3)*sin(2*theta2)*cos(theta1);

return {C1,
        C2,
        C3};
}

BLA::Matrix<1,3> computeG(float theta[3]){
  //Function for calculating the Gravity effect

  
  //defining thetas
float theta1=(theta[0]-180)*PI/180, theta2=(theta[1]-180)*PI/180, theta3=(theta[2]-180)*PI/180;
  //calcs
float G1 = 0.0016*cos(theta1) + 0.8318*sin(theta1)*sin(theta2) + 0.3274*cos(theta2)*sin(theta1)*sin(theta3) + 0.3274*cos(theta3)*sin(theta1)*sin(theta2);
float G2 = - 0.2747*cos(- 1.5693 + theta3 + theta2) - 0.1637*cos(0.0015 + theta3 + theta2 - 1*theta1) - 0.4159*cos(- 1*theta2 + theta1) - 0.1637*cos(0.0015 + theta3 + theta2 + theta1) - 0.6980*cos(- 1.5700 + theta2) - 0.4159*cos(theta2 + theta1);
float G3 = - 0.2747*cos(- 1.5693 + theta3 + theta2) - 0.1637*cos(0.0015 + theta3 + theta2 - 1*theta1) - 0.1637*cos(0.0015 + theta3 + theta2 + theta1);

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
  Thetaref[0] = Thetaref[0] + 180, Thetaref[1] = Thetaref[1] + 180, Thetaref[2] = Thetaref[2] + 180;

  //initializing variables
  float errTheta[3], errDTheta[3], errDDTheta[3];
              // Theta1|Theta2|theta3
  float kp[3] = { 64,   81,   100};
  float kd[3] = { 16,    18,    20};

  //Finding the angles and velocities
  for (int i = 0; i < 3; i++) {

    errTheta[i] = errorFunc(curTheta[i], Thetaref[i]);
    errDTheta[i] = errorFunc(curDTheta[i], dThetaref[i]);
    errDDTheta[i] = ddThetaref[i] + (kp[i] * errTheta[i]) + (kd[i] * errDTheta[i]);
  }
  //Serial.print("Error Theta 1: ");
  //Serial.println(errTheta[0]);


  
  //Serial.print("Error Theta 3: ");
  //Serial.println(errTheta[2]);

  //computing the dynamics
  BLA::Matrix<3, 3> H = computeH(curTheta);
  BLA::Matrix<1, 3> C = computeC(curTheta, curDTheta);
  BLA::Matrix<1, 3> G = computeG(curTheta);

  for (int i = 0; i < 3; i++) {
    //finding the H with control system
    float Hi = errH(H(0, i), H(1, i), H(2, i), errDDTheta); //H(currTheta)*(ddThetaref + kp*E + kd*dE)

    Q(0, i) = (Hi + C(0, i) + G(0, i));
    //setPWM(DXL_ID[i+1], Qi);
  }

 
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////
