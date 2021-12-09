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
    c2=2.9448;
  }else if(velocity = 0){
    c1=(138.81+40.4)/2;
    c2=2.9448;    
    }else{
    c1=40.4;
    c2=2.9448;
      }
  break;
  default:                  // For the MX-64 motor
    if(velocity > 0){
    c1=253.75;
    c2=4.8326;
  }else if(velocity = 0){
    c1=(253.75+80.9)/2;
    c2=4.8326;    
    }else{
    c1=80.9;
    c2=4.8326;
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
float H11=0.0014*cos(0.0015 + theta3 + 2*theta2 - 2*theta1) - 0.0044*cos(2*theta1) - 5.6000e-06*cos(- 1.5634 + theta2 + theta1) + 0.0014*cos(0.0015 + theta3 + 2*theta2 + 2*theta1) + 0.0016*cos(1.5671 - 2*theta3 - 2*theta2 + theta1) + 0.0016*cos(- 1.5671 + 2*theta3 + 2*theta2 + theta1) + 0.0047*cos(1.5693 - theta3 - 2*theta2 + theta1) - 0.0014*cos(0.0015 + theta3 - 2*theta1) - 0.0014*cos(0.0015 + theta3 + 2*theta1) - 0.0011*cos(0.0015 + theta3 + 2*theta2) + 5.6000e-06*cos(1.5634 - theta2 + theta1) + 0.0057*cos(1.5682 - 2*theta2 + theta1) + 0.0057*cos(- 1.5682 + 2*theta2 + theta1) + 3.3000e-06*cos(0.0074 + theta2 - 2*theta1) - 3.3000e-06*cos(0.0074 + theta2 + 2*theta1) + 0.0124*cos(0.0015 + theta3) + 4.8840e-04*cos(0.0037 + 2*theta3 + 2*theta2 - 2*theta1) + 4.8840e-04*cos(0.0037 + 2*theta3 + 2*theta2 + 2*theta1) + 0.0017*cos(0.0026 + 2*theta2 - 2*theta1) + 0.0017*cos(0.0026 + 2*theta2 + 2*theta1) - 3.9860e-04*cos(0.0037 + 2*theta3 + 2*theta2) + 0.0047*cos(- 1.5693 + theta3 + 2*theta2 + theta1) - 0.0014*cos(0.0026 + 2*theta2) + 0.0198;
float H12=4.3000e-06*cos(- 1.5634 + theta2 + theta1) + 0.0073*cos(- 1.5693 + theta3 + theta1) + 4.3000e-06*cos(1.5634 - theta2 + theta1) - 0.0073*cos(1.5693 - theta3 + theta1) - 7.3000e-06*cos(0.0074 + theta2) + 0.0230*sin(theta1);
float H13=0.0037*cos(- 1.5693 + theta3 + theta1) - 0.0037*cos(1.5693 - theta3 + theta1) + 0.0052*sin(theta1);
float H21=4.3000e-06*cos(- 1.5634 + theta2 + theta1) + 0.0073*cos(- 1.5693 + theta3 + theta1) + 4.3000e-06*cos(1.5634 - theta2 + theta1) - 0.0073*cos(1.5693 - theta3 + theta1) - 7.3000e-06*cos(0.0074 + theta2) + 0.0230*sin(theta1);
float H22=0.0191*cos(theta3) - 2.8700e-05*sin(theta3) - 1.0000e-05*pow(cos(theta1),2) + 1.0000e-05*pow(cos(theta1),4) + 1.0000e-05*pow(cos(theta1),2)*cos(theta3) - 1.0000e-05*pow(cos(theta1),4)*cos(theta3) + 8.0000e-06*pow(cos(theta1),2)*pow(cos(theta2),2) - 2.0000e-06*pow(cos(theta1),2)*pow(cos(theta3),2) - 8.0000e-06*pow(cos(theta1),4)*pow(cos(theta2),2) + 2.0000e-06*pow(cos(theta1),4)*pow(cos(theta3),2) + 4.0000e-06*pow(cos(theta1),2)*pow(cos(theta2),2)*pow(cos(theta3),2) - 4.0000e-06*pow(cos(theta1),4)*pow(cos(theta2),2)*pow(cos(theta3),2) - 1.0000e-05*pow(cos(theta1),2)*pow(cos(theta2),2)*cos(theta3) + 1.0000e-05*pow(cos(theta1),4)*pow(cos(theta2),2)*cos(theta3) + 1.0000e-05*pow(cos(theta1),2)*cos(theta2)*sin(theta2)*sin(theta3) - 1.0000e-05*pow(cos(theta1),4)*cos(theta2)*sin(theta2)*sin(theta3) + 0.0301;
float H23=0.0096*cos(theta3) - 1.4400e-05*sin(theta3) + 0.0067;
float H31=0.0037*cos(- 1.5693 + theta3 + theta1) - 0.0037*cos(1.5693 - theta3 + theta1) + 0.0052*sin(theta1);
float H32=0.0096*cos(theta3) - 1.4400e-05*sin(theta3) + 0.0067;
float H33=0.0067;
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
float C1=7.3000e-06*pow(dtheta2,2)*sin(theta2) + 0.0044*pow(dtheta1,2)*sin(2*theta1) - 1.8600e-05*dtheta1*dtheta3*cos(theta3) - 0.0124*dtheta1*dtheta3*sin(theta3) + 6.7000e-06*pow(dtheta1,2)*cos(2*theta1)*sin(theta2) - 1.5000e-05*pow(dtheta1,2)*cos(2*theta2)*sin(theta1) + 0.0028*pow(dtheta1,2)*sin(2*theta1)*cos(theta3) - 0.0057*pow(dtheta1,2)*sin(2*theta2)*sin(theta1) - 4.2000e-06*pow(dtheta1,2)*sin(2*theta1)*sin(theta3) + 7.3000e-06*dtheta1*dtheta2*cos(2*theta2) + 0.0028*dtheta1*dtheta2*sin(2*theta2) - 0.0034*pow(dtheta1,2)*cos(2*theta2)*sin(2*theta1) + 8.9000e-06*pow(dtheta1,2)*sin(2*theta1)*sin(2*theta2) - 5.6000e-06*pow(dtheta1,2)*cos(theta1)*cos(theta2) + 8.7000e-06*pow(dtheta2,2)*cos(theta1)*cos(theta2) - 1.1000e-05*pow(dtheta3,2)*cos(theta3)*sin(theta1) - 0.0073*pow(dtheta3,2)*sin(theta1)*sin(theta3) - 7.1000e-06*pow(dtheta1,2)*cos(2*theta2)*cos(theta3)*sin(theta1) - 0.0047*pow(dtheta1,2)*cos(2*theta2)*sin(theta1)*sin(theta3) - 0.0047*pow(dtheta1,2)*sin(2*theta2)*cos(theta3)*sin(theta1) + 0.0230*dtheta1*dtheta2*cos(2*theta2)*cos(theta1) + 3.4000e-06*dtheta1*dtheta2*cos(2*theta2)*cos(theta3) + 4.2000e-06*dtheta1*dtheta3*cos(2*theta1)*cos(theta3) + 1.7000e-06*dtheta1*dtheta3*cos(2*theta2)*cos(theta3) + 7.1000e-06*pow(dtheta1,2)*sin(2*theta2)*sin(theta1)*sin(theta3) + 6.7000e-06*dtheta1*dtheta2*sin(2*theta1)*cos(theta2) - 6.0000e-05*dtheta1*dtheta2*sin(2*theta2)*cos(theta1) + 0.0023*dtheta1*dtheta2*cos(2*theta2)*sin(theta3) + 0.0023*dtheta1*dtheta2*sin(2*theta2)*cos(theta3) + 0.0028*dtheta1*dtheta3*cos(2*theta1)*sin(theta3) + 0.0011*dtheta1*dtheta3*cos(2*theta2)*sin(theta3) + 0.0011*dtheta1*dtheta3*sin(2*theta2)*cos(theta3) - 3.4000e-06*dtheta1*dtheta2*sin(2*theta2)*sin(theta3) - 1.7000e-06*dtheta1*dtheta3*sin(2*theta2)*sin(theta3) - 6.1000e-06*pow(dtheta1,2)*cos(2*theta2)*cos(2*theta3)*sin(theta1) - 0.0028*pow(dtheta1,2)*cos(2*theta2)*sin(2*theta1)*cos(theta3) + 4.2000e-06*pow(dtheta1,2)*cos(2*theta2)*sin(2*theta1)*sin(theta3) - 0.0016*pow(dtheta1,2)*cos(2*theta2)*sin(2*theta3)*sin(theta1) - 0.0016*pow(dtheta1,2)*cos(2*theta3)*sin(2*theta2)*sin(theta1) + 4.2000e-06*pow(dtheta1,2)*sin(2*theta1)*sin(2*theta2)*cos(theta3) - 1.7900e-05*dtheta1*dtheta2*cos(2*theta1)*cos(2*theta2) + 3.0000e-06*dtheta1*dtheta2*cos(2*theta2)*cos(2*theta3) + 3.0000e-06*dtheta1*dtheta3*cos(2*theta2)*cos(2*theta3) + 0.0028*pow(dtheta1,2)*sin(2*theta1)*sin(2*theta2)*sin(theta3) + 6.1000e-06*pow(dtheta1,2)*sin(2*theta2)*sin(2*theta3)*sin(theta1) - 0.0068*dtheta1*dtheta2*cos(2*theta1)*sin(2*theta2) + 7.9680e-04*dtheta1*dtheta2*cos(2*theta2)*sin(2*theta3) + 7.9680e-04*dtheta1*dtheta2*cos(2*theta3)*sin(2*theta2) + 7.9680e-04*dtheta1*dtheta3*cos(2*theta2)*sin(2*theta3) + 7.9680e-04*dtheta1*dtheta3*cos(2*theta3)*sin(2*theta2) - 3.0000e-06*dtheta1*dtheta2*sin(2*theta2)*sin(2*theta3) - 3.0000e-06*dtheta1*dtheta3*sin(2*theta2)*sin(2*theta3) - 9.7680e-04*pow(dtheta1,2)*cos(2*theta2)*cos(2*theta3)*sin(2*theta1) - 2.2000e-05*dtheta2*dtheta3*cos(theta3)*sin(theta1) + 3.6000e-06*pow(dtheta1,2)*cos(2*theta2)*sin(2*theta1)*sin(2*theta3) + 3.6000e-06*pow(dtheta1,2)*cos(2*theta3)*sin(2*theta1)*sin(2*theta2) + 1.1200e-05*dtheta1*dtheta2*sin(theta1)*sin(theta2) - 0.0147*dtheta2*dtheta3*sin(theta1)*sin(theta3) + 9.7680e-04*pow(dtheta1,2)*sin(2*theta1)*sin(2*theta2)*sin(2*theta3) + 0.0188*dtheta1*dtheta2*cos(2*theta2)*cos(theta1)*cos(theta3) + 0.0094*dtheta1*dtheta3*cos(2*theta2)*cos(theta1)*cos(theta3) - 2.8300e-05*dtheta1*dtheta2*cos(2*theta2)*cos(theta1)*sin(theta3) - 2.8300e-05*dtheta1*dtheta2*sin(2*theta2)*cos(theta1)*cos(theta3) - 1.4100e-05*dtheta1*dtheta3*cos(2*theta2)*cos(theta1)*sin(theta3) - 1.4100e-05*dtheta1*dtheta3*sin(2*theta2)*cos(theta1)*cos(theta3) - 0.0188*dtheta1*dtheta2*sin(2*theta2)*cos(theta1)*sin(theta3) - 0.0094*dtheta1*dtheta3*sin(2*theta2)*cos(theta1)*sin(theta3) - 8.4000e-06*dtheta1*dtheta2*cos(2*theta1)*cos(2*theta2)*cos(theta3) + 0.0066*dtheta1*dtheta2*cos(2*theta2)*cos(2*theta3)*cos(theta1) - 4.2000e-06*dtheta1*dtheta3*cos(2*theta1)*cos(2*theta2)*cos(theta3) + 0.0066*dtheta1*dtheta3*cos(2*theta2)*cos(2*theta3)*cos(theta1) - 0.0056*dtheta1*dtheta2*cos(2*theta1)*cos(2*theta2)*sin(theta3) - 0.0056*dtheta1*dtheta2*cos(2*theta1)*sin(2*theta2)*cos(theta3) - 2.4400e-05*dtheta1*dtheta2*cos(2*theta2)*sin(2*theta3)*cos(theta1) - 2.4400e-05*dtheta1*dtheta2*cos(2*theta3)*sin(2*theta2)*cos(theta1) - 0.0028*dtheta1*dtheta3*cos(2*theta1)*cos(2*theta2)*sin(theta3) - 0.0028*dtheta1*dtheta3*cos(2*theta1)*sin(2*theta2)*cos(theta3) - 2.4400e-05*dtheta1*dtheta3*cos(2*theta2)*sin(2*theta3)*cos(theta1) - 2.4400e-05*dtheta1*dtheta3*cos(2*theta3)*sin(2*theta2)*cos(theta1) + 8.4000e-06*dtheta1*dtheta2*cos(2*theta1)*sin(2*theta2)*sin(theta3) - 0.0066*dtheta1*dtheta2*sin(2*theta2)*sin(2*theta3)*cos(theta1) + 4.2000e-06*dtheta1*dtheta3*cos(2*theta1)*sin(2*theta2)*sin(theta3) - 0.0066*dtheta1*dtheta3*sin(2*theta2)*sin(2*theta3)*cos(theta1) - 7.3000e-06*dtheta1*dtheta2*cos(2*theta1)*cos(2*theta2)*cos(2*theta3) - 7.3000e-06*dtheta1*dtheta3*cos(2*theta1)*cos(2*theta2)*cos(2*theta3) - 0.0020*dtheta1*dtheta2*cos(2*theta1)*cos(2*theta2)*sin(2*theta3) - 0.0020*dtheta1*dtheta2*cos(2*theta1)*cos(2*theta3)*sin(2*theta2) - 0.0020*dtheta1*dtheta3*cos(2*theta1)*cos(2*theta2)*sin(2*theta3) - 0.0020*dtheta1*dtheta3*cos(2*theta1)*cos(2*theta3)*sin(2*theta2) + 7.3000e-06*dtheta1*dtheta2*cos(2*theta1)*sin(2*theta2)*sin(2*theta3) + 7.3000e-06*dtheta1*dtheta3*cos(2*theta1)*sin(2*theta2)*sin(2*theta3);
float C2=0.0230*pow(dtheta1,2)*cos(theta1) - 3.7000e-06*pow(dtheta1,2)*cos(2*theta2) - 0.0014*pow(dtheta1,2)*sin(2*theta2) - 0.0096*pow(dtheta3,2)*sin(theta3) - 1.4400e-05*pow(dtheta3,2)*cos(theta3) - 2.8700e-05*dtheta2*dtheta3*cos(theta3) - 0.0191*dtheta2*dtheta3*sin(theta3) - 0.0115*pow(dtheta1,2)*cos(2*theta2)*cos(theta1) - 1.7000e-06*pow(dtheta1,2)*cos(2*theta2)*cos(theta3) - 3.3000e-06*pow(dtheta1,2)*sin(2*theta1)*cos(theta2) + 3.0000e-05*pow(dtheta1,2)*sin(2*theta2)*cos(theta1) - 0.0011*pow(dtheta1,2)*cos(2*theta2)*sin(theta3) - 0.0011*pow(dtheta1,2)*sin(2*theta2)*cos(theta3) + 1.7000e-06*pow(dtheta1,2)*sin(2*theta2)*sin(theta3) + 8.9000e-06*pow(dtheta1,2)*cos(2*theta1)*cos(2*theta2) - 1.5000e-06*pow(dtheta1,2)*cos(2*theta2)*cos(2*theta3) + 0.0034*pow(dtheta1,2)*cos(2*theta1)*sin(2*theta2) - 3.9870e-04*pow(dtheta1,2)*cos(2*theta2)*sin(2*theta3) - 3.9870e-04*pow(dtheta1,2)*cos(2*theta3)*sin(2*theta2) + 1.5000e-06*pow(dtheta1,2)*sin(2*theta2)*sin(2*theta3) + 0.0147*pow(dtheta1,2)*cos(theta1)*cos(theta3) - 2.2000e-05*pow(dtheta1,2)*cos(theta1)*sin(theta3) - 1.4300e-05*pow(dtheta1,2)*sin(theta1)*sin(theta2) - 0.0094*pow(dtheta1,2)*cos(2*theta2)*cos(theta1)*cos(theta3) + 1.4100e-05*pow(dtheta1,2)*cos(2*theta2)*cos(theta1)*sin(theta3) + 1.4100e-05*pow(dtheta1,2)*sin(2*theta2)*cos(theta1)*cos(theta3) + 0.0094*pow(dtheta1,2)*sin(2*theta2)*cos(theta1)*sin(theta3) + 4.2000e-06*pow(dtheta1,2)*cos(2*theta1)*cos(2*theta2)*cos(theta3) - 0.0033*pow(dtheta1,2)*cos(2*theta2)*cos(2*theta3)*cos(theta1) + 0.0028*pow(dtheta1,2)*cos(2*theta1)*cos(2*theta2)*sin(theta3) + 0.0028*pow(dtheta1,2)*cos(2*theta1)*sin(2*theta2)*cos(theta3) + 1.2200e-05*pow(dtheta1,2)*cos(2*theta2)*sin(2*theta3)*cos(theta1) + 1.2200e-05*pow(dtheta1,2)*cos(2*theta3)*sin(2*theta2)*cos(theta1) - 4.2000e-06*pow(dtheta1,2)*cos(2*theta1)*sin(2*theta2)*sin(theta3) + 0.0033*pow(dtheta1,2)*sin(2*theta2)*sin(2*theta3)*cos(theta1) + 3.6000e-06*pow(dtheta1,2)*cos(2*theta1)*cos(2*theta2)*cos(2*theta3) + 9.7680e-04*pow(dtheta1,2)*cos(2*theta1)*cos(2*theta2)*sin(2*theta3) + 9.7680e-04*pow(dtheta1,2)*cos(2*theta1)*cos(2*theta3)*sin(2*theta2) - 2.2000e-05*dtheta1*dtheta3*cos(theta3)*sin(theta1) - 3.6000e-06*pow(dtheta1,2)*cos(2*theta1)*sin(2*theta2)*sin(2*theta3) - 0.0147*dtheta1*dtheta3*sin(theta1)*sin(theta3);
float C3=1.1000e-06*pow(dtheta1,2)*cos(theta3 + 2*theta2 - 2*theta1) - 0.0016*pow(dtheta1,2)*cos(2*theta3 + 2*theta2 + theta1) - 0.0024*pow(dtheta1,2)*cos(theta3 + 2*theta2 - theta1) + 1.1000e-06*pow(dtheta1,2)*cos(theta3 + 2*theta2 + 2*theta1) + 0.0062*pow(dtheta1,2)*sin(theta3) + 0.0096*pow(dtheta2,2)*sin(theta3) + 3.5000e-06*pow(dtheta1,2)*sin(theta3 + 2*theta2 - theta1) + 6.1000e-06*pow(dtheta1,2)*sin(2*theta3 + 2*theta2 + theta1) + 7.0200e-04*pow(dtheta1,2)*sin(theta3 + 2*theta2 - 2*theta1) + 7.0200e-04*pow(dtheta1,2)*sin(theta3 + 2*theta2 + 2*theta1) + 0.0037*pow(dtheta1,2)*cos(- theta3 + theta1) - 1.1000e-06*pow(dtheta1,2)*cos(theta3 + 2*theta1) + 5.5000e-06*pow(dtheta1,2)*sin(- theta3 + theta1) - 7.0200e-04*pow(dtheta1,2)*sin(theta3 + 2*theta1) - 5.7300e-04*pow(dtheta1,2)*sin(theta3 + 2*theta2) - 0.0016*pow(dtheta1,2)*cos(2*theta3 + 2*theta2 - theta1) + 1.8000e-06*pow(dtheta1,2)*cos(2*theta3 + 2*theta2 - 2*theta1) + 1.8000e-06*pow(dtheta1,2)*cos(2*theta3 + 2*theta2 + 2*theta1) + 6.1000e-06*pow(dtheta1,2)*sin(2*theta3 + 2*theta2 - theta1) + 4.8840e-04*pow(dtheta1,2)*sin(2*theta3 + 2*theta2 - 2*theta1) + 4.8840e-04*pow(dtheta1,2)*sin(2*theta3 + 2*theta2 + 2*theta1) - 1.1000e-06*pow(dtheta1,2)*cos(- theta3 + 2*theta1) - 1.5000e-06*pow(dtheta1,2)*cos(2*theta3 + 2*theta2) + 7.0200e-04*pow(dtheta1,2)*sin(- theta3 + 2*theta1) - 3.9870e-04*pow(dtheta1,2)*sin(2*theta3 + 2*theta2) - 0.0024*pow(dtheta1,2)*cos(theta3 + 2*theta2 + theta1) + 3.5000e-06*pow(dtheta1,2)*sin(theta3 + 2*theta2 + theta1) + 0.0037*pow(dtheta1,2)*cos(theta3 + theta1) - 5.5000e-06*pow(dtheta1,2)*sin(theta3 + theta1) + 0.0052*pow(dtheta1,2)*cos(theta1) + 9.3000e-06*pow(dtheta1,2)*cos(theta3) + 1.4400e-05*pow(dtheta2,2)*cos(theta3) + 0.0073*dtheta1*dtheta2*cos(- theta3 + theta1) + 1.1000e-05*dtheta1*dtheta2*sin(- theta3 + theta1) - 0.0073*dtheta1*dtheta2*cos(theta3 + theta1) + 1.1000e-05*dtheta1*dtheta2*sin(theta3 + theta1);

return {C1,
        C2,
        C3};
}

BLA::Matrix<1,3> computeG(float theta[3]){
  //Function for calculating the Gravity effect

  
  //defining thetas
float theta1=(theta[0]-180)*PI/180, theta2=(theta[1]-180)*PI/180, theta3=(theta[2]-180)*PI/180;
  //calcs
float G1 = 0.4159*cos(theta2 - theta1 + 0.0011) + 0.1637*cos(theta2 - theta1 + theta3 + 0.0015) + 0.0013*cos(theta1) - 0.4159*cos(theta1 + theta2 + 0.0011) - 0.1637*cos(theta1 + theta2 + theta3 + 0.0015);
float G2 = -0.6980*cos(theta2 - 1.5697) - 0.4159*cos(theta2 - theta1 + 0.0011) - 0.1637*cos(theta2 - theta1 + theta3 + 0.0015) - 0.4159*cos(theta1 + theta2 + 0.0011) - 0.1637*cos(theta1 + theta2 + theta3 + 0.0015) - 0.2747*cos(theta2 + theta3 - 1.5693);
float G3 = -0.1637*cos(theta2 - theta1 + theta3 + 0.0015) - 0.1637*cos(theta1 + theta2 + theta3 + 0.0015) - 0.2747*cos(theta2 + theta3 - 1.5693);

return {G1,
        G2,
        G3};
}



float errorFunc(float desired,float measured){ //error function
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
  //Thetaref[0] = Thetaref[0] + 180, Thetaref[1] = Thetaref[1] + 180, Thetaref[2] = Thetaref[2] + 180;

  //initializing variables
  float errTheta[3], errDTheta[3], errDDTheta[3];
// Theta1|Theta2|theta3
  float kp[3] = {196, 400, 900};
  float kd[3] = {28, 40, 60};

  //Finding the angles and velocities
  for (int i = 0; i < 3; i++) {
    errTheta[i] = errorFunc(curTheta[i]-180, Thetaref[i]);
    errDTheta[i] = errorFunc(curDTheta[i], dThetaref[i]);
    errDDTheta[i] = ddThetaref[i] + (kp[i] * errTheta[i]) + (kd[i] * errDTheta[i]);
  }

       /* PRINT_VALUE("\n Error_Theta1:\t",errTheta[0]);
        PRINT_VALUE(" Error_dTheta1:\t",errDTheta[0]);
        PRINT_VALUE(" Error_Theta2:\t",errTheta[1]);
        PRINT_VALUE(" Error_dTheta2:\t",errDTheta[1]);
        PRINT_VALUE(" Error_Theta3:\t",errTheta[2]);
        PRINT_VALUE(" Error_dTheta3:\t",errDTheta[2]);*/

  //computing the dynamics
  BLA::Matrix<3, 3> H = computeH(curTheta);
  BLA::Matrix<1, 3> C = computeC(curTheta, curDTheta);
  BLA::Matrix<1, 3> G = computeG(curTheta);

  for (int i = 0; i < 3; i++) {
    //finding the H with control system
    float Hi = errH(H(i, 0), H(i, 1), H(i, 2), errDDTheta); //H(currTheta)*(ddThetaref + kp*E + kd*dE)

    Q(0, i) = (Hi + C(0, i) + G(0, i));
  }
  PRINT_VALUE(" Q1:\t",Q(0, 0));
  PRINT_VALUE(" Q2:\t",Q(0, 1));
  PRINT_VALUE(" Q3:\t",Q(0, 2));

 
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////
