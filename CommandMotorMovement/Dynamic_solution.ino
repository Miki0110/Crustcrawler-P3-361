BLA::Matrix<3,3> computeH(float theta[3]){
//Function for calculating the mass matrix


//defining thetas
float theta1=(theta[0]-190)*PI/180, theta2=(theta[1]-180)*PI/180, theta3=(theta[2]-180)*PI/180;

//defing the mass matrix
float H11=0.157*sin(2.0*theta2 - 1.0*theta1 + 2.0*theta3) - 0.0042*sin(theta1 + theta2 + theta3) - 0.0019*cos(theta1 - 2.0*theta2) - 0.0019*cos(theta1 + 2.0*theta2) - 0.00251*cos(2.0*theta1 + theta2) - 0.0014*cos(2.0*theta1 + theta3) - 0.00115*cos(2.0*theta2 + theta3) - 0.00421*sin(theta1 - 1.0*theta2) - 0.161*sin(theta1 - 2.0*theta2) + 0.161*sin(theta1 + 2.0*theta2) - 0.0019*cos(theta1 + 2.0*theta2 + 2.0*theta3) + 0.0014*cos(2.0*theta2 - 2.0*theta1 + theta3) + 0.0014*cos(2.0*theta1 + 2.0*theta2 + theta3) + 0.00471*sin(2.0*theta2 - 1.0*theta1 + theta3) + 0.157*sin(theta1 + 2.0*theta2 + 2.0*theta3) - 0.208*cos(2.0*theta1) - 0.0392*cos(2.0*theta2) - 0.00421*sin(theta1 + theta2) + 0.0025*cos(theta2 - 2.0*theta1 + theta3) - 0.0025*cos(2.0*theta1 + theta2 + theta3) + 0.0042*sin(theta2 - 1.0*theta1 + theta3) + 0.00471*sin(theta1 + 2.0*theta2 + theta3) - 0.00279*cos(theta1) + 0.0124*cos(theta3) + 0.00251*cos(2.0*theta1 - 1.0*theta2) + 0.048*cos(2.0*theta1 - 2.0*theta2) + 0.048*cos(2.0*theta1 + 2.0*theta2) - 0.0014*cos(2.0*theta1 - 1.0*theta3) - 0.0382*cos(2.0*theta2 + 2.0*theta3) - 0.0019*cos(2.0*theta2 - 1.0*theta1 + 2.0*theta3) + 0.0467*cos(2.0*theta2 - 2.0*theta1 + 2.0*theta3) + 0.0467*cos(2.0*theta1 + 2.0*theta2 + 2.0*theta3) + 1.08;
float H12=0.00326*sin(theta1 + theta2 + theta3) - 0.00327*sin(theta1 - 1.0*theta2) + 0.00733*sin(theta1 - 1.0*theta3) - 0.00548*cos(theta2 + theta3) + 0.00327*sin(theta1 + theta2) + 0.00733*sin(theta1 + theta3) + 0.00326*sin(theta2 - 1.0*theta1 + theta3) - 0.00549*cos(theta2) + 1.11*sin(theta1);
float H13=0.00326*sin(theta1 + theta2 + theta3) + 0.00366*sin(theta1 - 1.0*theta3) - 0.00548*cos(theta2 + theta3) + 0.00366*sin(theta1 + theta3) + 0.00326*sin(theta2 - 1.0*theta1 + theta3) + 0.546*sin(theta1);

float H21=0.00326*sin(theta1 + theta2 + theta3) - 0.00327*sin(theta1 - 1.0*theta2) + 0.00733*sin(theta1 - 1.0*theta3) - 0.00548*cos(theta2 + theta3) + 0.00327*sin(theta1 + theta2) + 0.00733*sin(theta1 + theta3) + 0.00326*sin(theta2 - 1.0*theta1 + theta3) - 0.00549*cos(theta2) + 1.11*sin(theta1);
float H22=0.0191*cos(theta3) + 1.44;
float H23=0.00957*cos(theta3) + 0.713;

float H31=0.00326*sin(theta1 + theta2 + theta3) + 0.00366*sin(theta1 - 1.0*theta3) - 0.00548*cos(theta2 + theta3) + 0.00366*sin(theta1 + theta3) + 0.00326*sin(theta2 - 1.0*theta1 + theta3) + 0.546*sin(theta1);
float H32=0.00957*cos(theta3) + 0.713;
float H33=0.713;
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
float C1=0.0014*pow(dtheta1,2)*sin(theta1) + 0.00549*pow(dtheta2,2)*sin(theta2) + 0.208*pow(dtheta1,2)*sin(2.0*theta1) - 0.0124*dtheta1*dtheta3*sin(theta3) - 0.0959*pow(dtheta1,2)*cos(2.0*theta2)*sin(2.0*theta1) - 0.00113*pow(dtheta1,2)*sin(2.0*theta1)*sin(2.0*theta2) + 0.00502*pow(dtheta1,2)*cos(2.0*theta1)*sin(theta2) + 0.0019*pow(dtheta1,2)*cos(2.0*theta2)*sin(theta1) + 0.00281*pow(dtheta1,2)*sin(2.0*theta1)*cos(theta3) - 0.161*pow(dtheta1,2)*sin(2.0*theta2)*sin(theta1) + 0.0783*dtheta1*dtheta2*sin(2.0*theta2) - 0.00421*pow(dtheta1,2)*cos(theta1)*cos(theta2) + 0.00655*pow(dtheta2,2)*cos(theta1)*cos(theta2) + 0.00548*pow(dtheta2,2)*cos(theta2)*sin(theta3) + 0.00548*pow(dtheta2,2)*cos(theta3)*sin(theta2) + 0.00548*pow(dtheta3,2)*cos(theta2)*sin(theta3) + 0.00548*pow(dtheta3,2)*cos(theta3)*sin(theta2) - 0.00733*pow(dtheta3,2)*sin(theta1)*sin(theta3) - 0.157*pow(dtheta1,2)*cos(2.0*theta2)*sin(2.0*theta3)*sin(theta1) - 0.157*pow(dtheta1,2)*cos(2.0*theta3)*sin(2.0*theta2)*sin(theta1) + 0.00226*dtheta1*dtheta2*cos(2.0*theta1)*cos(2.0*theta2) + 0.00281*pow(dtheta1,2)*sin(2.0*theta1)*sin(2.0*theta2)*sin(theta3) - 0.0019*pow(dtheta1,2)*sin(2.0*theta2)*sin(2.0*theta3)*sin(theta1) - 0.192*dtheta1*dtheta2*cos(2.0*theta1)*sin(2.0*theta2) + 0.0763*dtheta1*dtheta2*cos(2.0*theta2)*sin(2.0*theta3) + 0.0763*dtheta1*dtheta2*cos(2.0*theta3)*sin(2.0*theta2) + 0.0763*dtheta1*dtheta3*cos(2.0*theta2)*sin(2.0*theta3) + 0.0763*dtheta1*dtheta3*cos(2.0*theta3)*sin(2.0*theta2) + 0.005*pow(dtheta1,2)*cos(2.0*theta1)*cos(theta2)*sin(theta3) + 0.005*pow(dtheta1,2)*cos(2.0*theta1)*cos(theta3)*sin(theta2) - 0.00471*pow(dtheta1,2)*cos(2.0*theta2)*sin(theta1)*sin(theta3) - 0.00471*pow(dtheta1,2)*sin(2.0*theta2)*cos(theta3)*sin(theta1) + 0.644*dtheta1*dtheta2*cos(2.0*theta2)*cos(theta1) + 0.00502*dtheta1*dtheta2*sin(2.0*theta1)*cos(theta2) + 0.00759*dtheta1*dtheta2*sin(2.0*theta2)*cos(theta1) + 0.00229*dtheta1*dtheta2*cos(2.0*theta2)*sin(theta3) + 0.00229*dtheta1*dtheta2*sin(2.0*theta2)*cos(theta3) + 0.00281*dtheta1*dtheta3*cos(2.0*theta1)*sin(theta3) + 0.00115*dtheta1*dtheta3*cos(2.0*theta2)*sin(theta3) + 0.00115*dtheta1*dtheta3*sin(2.0*theta2)*cos(theta3) - 0.0935*pow(dtheta1,2)*cos(2.0*theta2)*cos(2.0*theta3)*sin(2.0*theta1) - 0.00113*pow(dtheta1,2)*cos(2.0*theta2)*sin(2.0*theta1)*sin(2.0*theta3) - 0.00113*pow(dtheta1,2)*cos(2.0*theta3)*sin(2.0*theta1)*sin(2.0*theta2) - 0.0042*pow(dtheta1,2)*cos(theta1)*cos(theta2)*cos(theta3) + 0.00653*pow(dtheta2,2)*cos(theta1)*cos(theta2)*cos(theta3) + 0.00653*pow(dtheta3,2)*cos(theta1)*cos(theta2)*cos(theta3) + 0.0935*pow(dtheta1,2)*sin(2.0*theta1)*sin(2.0*theta2)*sin(2.0*theta3) + 0.0042*pow(dtheta1,2)*cos(theta1)*sin(theta2)*sin(theta3) - 0.00653*pow(dtheta2,2)*cos(theta1)*sin(theta2)*sin(theta3) - 0.00653*pow(dtheta3,2)*cos(theta1)*sin(theta2)*sin(theta3) + 0.011*dtheta2*dtheta3*cos(theta2)*sin(theta3) + 0.011*dtheta2*dtheta3*cos(theta3)*sin(theta2) + 0.00842*dtheta1*dtheta2*sin(theta1)*sin(theta2) - 0.0147*dtheta2*dtheta3*sin(theta1)*sin(theta3) + 0.0019*pow(dtheta1,2)*cos(2.0*theta2)*cos(2.0*theta3)*sin(theta1) - 0.00281*pow(dtheta1,2)*cos(2.0*theta2)*sin(2.0*theta1)*cos(theta3) - 0.628*dtheta1*dtheta2*sin(2.0*theta2)*sin(2.0*theta3)*cos(theta1) - 0.628*dtheta1*dtheta3*sin(2.0*theta2)*sin(2.0*theta3)*cos(theta1) + 0.0188*dtheta1*dtheta2*cos(2.0*theta2)*cos(theta1)*cos(theta3) + 0.00942*dtheta1*dtheta3*cos(2.0*theta2)*cos(theta1)*cos(theta3) + 0.005*dtheta1*dtheta2*sin(2.0*theta1)*cos(theta2)*cos(theta3) + 0.005*dtheta1*dtheta3*sin(2.0*theta1)*cos(theta2)*cos(theta3) - 0.0188*dtheta1*dtheta2*sin(2.0*theta2)*cos(theta1)*sin(theta3) - 0.00942*dtheta1*dtheta3*sin(2.0*theta2)*cos(theta1)*sin(theta3) - 0.005*dtheta1*dtheta2*sin(2.0*theta1)*sin(theta2)*sin(theta3) - 0.005*dtheta1*dtheta3*sin(2.0*theta1)*sin(theta2)*sin(theta3) + 0.00227*dtheta1*dtheta2*cos(2.0*theta1)*cos(2.0*theta2)*cos(2.0*theta3) + 0.00227*dtheta1*dtheta3*cos(2.0*theta1)*cos(2.0*theta2)*cos(2.0*theta3) - 0.187*dtheta1*dtheta2*cos(2.0*theta1)*cos(2.0*theta2)*sin(2.0*theta3) - 0.187*dtheta1*dtheta2*cos(2.0*theta1)*cos(2.0*theta3)*sin(2.0*theta2) - 0.187*dtheta1*dtheta3*cos(2.0*theta1)*cos(2.0*theta2)*sin(2.0*theta3) - 0.187*dtheta1*dtheta3*cos(2.0*theta1)*cos(2.0*theta3)*sin(2.0*theta2) - 0.00227*dtheta1*dtheta2*cos(2.0*theta1)*sin(2.0*theta2)*sin(2.0*theta3) - 0.00227*dtheta1*dtheta3*cos(2.0*theta1)*sin(2.0*theta2)*sin(2.0*theta3) + 0.0131*dtheta2*dtheta3*cos(theta1)*cos(theta2)*cos(theta3) + 0.00839*dtheta1*dtheta2*cos(theta2)*sin(theta1)*sin(theta3) + 0.00839*dtheta1*dtheta2*cos(theta3)*sin(theta1)*sin(theta2) + 0.00839*dtheta1*dtheta3*cos(theta2)*sin(theta1)*sin(theta3) + 0.00839*dtheta1*dtheta3*cos(theta3)*sin(theta1)*sin(theta2) - 0.0131*dtheta2*dtheta3*cos(theta1)*sin(theta2)*sin(theta3) + 0.628*dtheta1*dtheta2*cos(2.0*theta2)*cos(2.0*theta3)*cos(theta1) + 0.628*dtheta1*dtheta3*cos(2.0*theta2)*cos(2.0*theta3)*cos(theta1) - 0.00561*dtheta1*dtheta2*cos(2.0*theta1)*cos(2.0*theta2)*sin(theta3) - 0.00561*dtheta1*dtheta2*cos(2.0*theta1)*sin(2.0*theta2)*cos(theta3) + 0.00762*dtheta1*dtheta2*cos(2.0*theta2)*sin(2.0*theta3)*cos(theta1) + 0.00762*dtheta1*dtheta2*cos(2.0*theta3)*sin(2.0*theta2)*cos(theta1) - 0.00281*dtheta1*dtheta3*cos(2.0*theta1)*cos(2.0*theta2)*sin(theta3) - 0.00281*dtheta1*dtheta3*cos(2.0*theta1)*sin(2.0*theta2)*cos(theta3) + 0.00762*dtheta1*dtheta3*cos(2.0*theta2)*sin(2.0*theta3)*cos(theta1) + 0.00762*dtheta1*dtheta3*cos(2.0*theta3)*sin(2.0*theta2)*cos(theta1);
float C2=1.11*pow(dtheta1,2)*cos(theta1) - 0.0392*pow(dtheta1,2)*sin(2.0*theta2) - 0.00957*pow(dtheta3,2)*sin(theta3) - 0.0191*dtheta2*dtheta3*sin(theta3) - 0.00113*pow(dtheta1,2)*cos(2.0*theta1)*cos(2.0*theta2) + 0.0959*pow(dtheta1,2)*cos(2.0*theta1)*sin(2.0*theta2) - 0.0382*pow(dtheta1,2)*cos(2.0*theta2)*sin(2.0*theta3) - 0.0382*pow(dtheta1,2)*cos(2.0*theta3)*sin(2.0*theta2) - 0.322*pow(dtheta1,2)*cos(2.0*theta2)*cos(theta1) - 0.00251*pow(dtheta1,2)*sin(2.0*theta1)*cos(theta2) - 0.00379*pow(dtheta1,2)*sin(2.0*theta2)*cos(theta1) - 0.00115*pow(dtheta1,2)*cos(2.0*theta2)*sin(theta3) - 0.00115*pow(dtheta1,2)*sin(2.0*theta2)*cos(theta3) + 0.0147*pow(dtheta1,2)*cos(theta1)*cos(theta3) - 0.0108*pow(dtheta1,2)*sin(theta1)*sin(theta2) + 0.314*pow(dtheta1,2)*sin(2.0*theta2)*sin(2.0*theta3)*cos(theta1) - 0.00942*pow(dtheta1,2)*cos(2.0*theta2)*cos(theta1)*cos(theta3) - 0.0025*pow(dtheta1,2)*sin(2.0*theta1)*cos(theta2)*cos(theta3) + 0.00942*pow(dtheta1,2)*sin(2.0*theta2)*cos(theta1)*sin(theta3) + 0.0025*pow(dtheta1,2)*sin(2.0*theta1)*sin(theta2)*sin(theta3) - 0.00113*pow(dtheta1,2)*cos(2.0*theta1)*cos(2.0*theta2)*cos(2.0*theta3) + 0.0935*pow(dtheta1,2)*cos(2.0*theta1)*cos(2.0*theta2)*sin(2.0*theta3) + 0.0935*pow(dtheta1,2)*cos(2.0*theta1)*cos(2.0*theta3)*sin(2.0*theta2) + 0.00113*pow(dtheta1,2)*cos(2.0*theta1)*sin(2.0*theta2)*sin(2.0*theta3) - 0.0107*pow(dtheta1,2)*cos(theta2)*sin(theta1)*sin(theta3) - 0.0107*pow(dtheta1,2)*cos(theta3)*sin(theta1)*sin(theta2) - 0.314*pow(dtheta1,2)*cos(2.0*theta2)*cos(2.0*theta3)*cos(theta1) - 0.0147*dtheta1*dtheta3*sin(theta1)*sin(theta3) + 0.00281*pow(dtheta1,2)*cos(2.0*theta1)*cos(2.0*theta2)*sin(theta3) + 0.00281*pow(dtheta1,2)*cos(2.0*theta1)*sin(2.0*theta2)*cos(theta3) - 0.00381*pow(dtheta1,2)*cos(2.0*theta2)*sin(2.0*theta3)*cos(theta1) - 0.00381*pow(dtheta1,2)*cos(2.0*theta3)*sin(2.0*theta2)*cos(theta1);
float C3=0.00619*pow(dtheta1,2)*sin(theta3) + 0.00957*pow(dtheta2,2)*sin(theta3) + 0.546*pow(dtheta1,2)*cos(theta1) - 0.0382*pow(dtheta1,2)*cos(2.0*theta2)*sin(2.0*theta3) - 0.0382*pow(dtheta1,2)*cos(2.0*theta3)*sin(2.0*theta2) - 0.0014*pow(dtheta1,2)*cos(2.0*theta1)*sin(theta3) + 0.00733*pow(dtheta1,2)*cos(theta1)*cos(theta3) + 0.314*pow(dtheta1,2)*sin(2.0*theta2)*sin(2.0*theta3)*cos(theta1) - 0.00471*pow(dtheta1,2)*cos(2.0*theta2)*cos(theta1)*cos(theta3) - 0.0025*pow(dtheta1,2)*sin(2.0*theta1)*cos(theta2)*cos(theta3) + 0.00471*pow(dtheta1,2)*sin(2.0*theta2)*cos(theta1)*sin(theta3) + 0.0025*pow(dtheta1,2)*sin(2.0*theta1)*sin(theta2)*sin(theta3) - 0.00113*pow(dtheta1,2)*cos(2.0*theta1)*cos(2.0*theta2)*cos(2.0*theta3) + 0.0935*pow(dtheta1,2)*cos(2.0*theta1)*cos(2.0*theta2)*sin(2.0*theta3) + 0.0935*pow(dtheta1,2)*cos(2.0*theta1)*cos(2.0*theta3)*sin(2.0*theta2) + 0.00113*pow(dtheta1,2)*cos(2.0*theta1)*sin(2.0*theta2)*sin(2.0*theta3) - 0.0107*pow(dtheta1,2)*cos(theta2)*sin(theta1)*sin(theta3) - 0.0107*pow(dtheta1,2)*cos(theta3)*sin(theta1)*sin(theta2) - 0.314*pow(dtheta1,2)*cos(2.0*theta2)*cos(2.0*theta3)*cos(theta1) + 0.0147*dtheta1*dtheta2*sin(theta1)*sin(theta3) + 0.0014*pow(dtheta1,2)*cos(2.0*theta1)*cos(2.0*theta2)*sin(theta3) + 0.0014*pow(dtheta1,2)*cos(2.0*theta1)*sin(2.0*theta2)*cos(theta3) - 0.00381*pow(dtheta1,2)*cos(2.0*theta2)*sin(2.0*theta3)*cos(theta1) - 0.00381*pow(dtheta1,2)*cos(2.0*theta3)*sin(2.0*theta2)*cos(theta1);

return {C1,
        C2,
        C3};
}

BLA::Matrix<1,3> computeG(float theta[3]){
  //Function for calculating the Gravity effect

  
  //defining thetas
float theta1=(theta[0]-190)*PI/180, theta2=(theta[1]-180)*PI/180, theta3=(theta[2]-180)*PI/180;
  //calcs
float G1 = - 0.0014*cos(theta1) - 0.832*sin(theta1)*sin(theta2) - 0.327*cos(theta2)*sin(theta1)*sin(theta3) - 0.327*cos(theta3)*sin(theta1)*sin(theta2);
float G2 = 0.698*sin(theta2) + 0.832*cos(theta1)*cos(theta2) + 0.275*cos(theta2)*sin(theta3) + 0.275*cos(theta3)*sin(theta2) - 0.327*cos(theta1)*sin(theta2)*sin(theta3) + 0.327*cos(theta1)*cos(theta2)*cos(theta3);
float G3 = 0.275*cos(theta2)*sin(theta3) + 0.275*cos(theta3)*sin(theta2) - 0.327*cos(theta1)*sin(theta2)*sin(theta3) + 0.327*cos(theta1)*cos(theta2)*cos(theta3);

return {-G1,
        -G2,
        -G3};
}

float errH(float H1, float H2, float H3, float ddtheta[3]){
  //acceleration
float ddtheta1=ddtheta[0]*PI/180,ddtheta2=ddtheta[1]*PI/180,ddtheta3=ddtheta[2]*PI/180;

  return H1*ddtheta1+H2*ddtheta2+H3*ddtheta3;
  }



float errorFunc(float measured, float desired){ //error function
  return desired - measured;
}

float getangle(const uint8_t id){
  return dxl.getPresentPosition(id, UNIT_DEGREE);
  }

float getvelocity(const uint8_t id){
  return dxl.getPresentVelocity(id, UNIT_RPM)/360*60;
  }

void updateRef(){ //Function for updating the references (should be set up with trajectory later)
  }

void control(const uint8_t id, float Thetaref, float dThetaref, float ddThetaref) {
  Thetaref=Thetaref+180;
  
  float kp = 144;
  float kd = 24;
 
  float curAngle = getangle(id);
  float curAngle2 = getangle(id+1);
  float curAngle3 = getangle(id-1);
  Serial.print("Current angle2: ");
  Serial.println(curAngle);
  Serial.print("Current angle3: ");
  Serial.println(curAngle2);
  Serial.print("Current angle1: ");
  Serial.println(curAngle3);
  float curVel = getvelocity(id);
 
  float curTheta[3] = {190.0,curAngle,180.0};
  float curDTheta[3] = {0,curVel,0};

  float errTheta = errorFunc(curAngle, Thetaref);
  float errVel = errorFunc(curVel, dThetaref);
  
  BLA::Matrix<3,3> H = computeH(curTheta);
  BLA::Matrix<1,3> C = computeC(curTheta,curDTheta);
  BLA::Matrix<1,3> G = computeG(curTheta);
  
  Serial.print("Current errTheta: ");
  Serial.println(errTheta);
  float errDDTheta = ddThetaref + (kp * errTheta) + (kd * errVel);
  
  float ddTheta[3]= {0,errDDTheta,0};

  float H2 = errH(H(1,0), H(1,1), H(1,2), ddTheta);
  //Serial.print("Current H2: ");
  //Serial.println(H2);
  
  float Q = H2+C(0,1)+G(0,1);
  //Serial.print("Current torque: ");
  //Serial.println(Q);

  //setMotorTorque(DXL_ID[1], -torque(0,0));
  setPWM(id, Q);
  //setMotorTorque(DXL_ID[3], -torque(0,2));
  
 // delay(500);
}
