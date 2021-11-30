//Link lengths (in mm)
const double Lrx = -124.70;//X coordinate reference to joint 1 motor (base)
const double Lry = 143.70;//Y coordinate reference to joint 1 motor (base)
const double Lrz = 254; //Z coordinate reference to joint 1 motor (base)
const double Lra = 50;//Base tilt angle compared to reference (in deg)

const double Lb =  60.80; //Base link (motor 2)
const double L1 = 219.80; //Link 1
const double L2 = 147.30 + 126; //Link 2 + gripper distance
//const double Lg = 145.14; //From gripper joints to grip point
const double BasicValue1 = 191.6; //In degree values
const double BasicValue2 = 180; //In degree values
const double BasicValue3 = 180; //In degree values


double theta1;
double theta2;
double theta3;

//4x4 Homogenous transformation matricies from crustcrawler
BLA::Matrix<4, 4> TRB = {
  cos(Lra*PI / 180), 0, sin(Lra*PI / 180),  Lrx,
  0, 1,               0,  Lry,
  -sin(Lra*PI / 180), 0, cos(Lra*PI / 180),  Lrz,
  0, 0,               0,  1
};
BLA::Matrix<4, 4> TB0 = {
  1, 0, 0, 0,
  0, 1, 0, 0,
  0, 0, 1, Lb,
  0, 0, 0, 1
};
BLA::Matrix<4, 4> CalcT01(double theta1ins) {
  return {cos(theta1ins), -sin(theta1ins), 0, 0,
          sin(theta1ins),  cos(theta1ins), 0, 0,
          0,            0, 1, 0,
          0,            0, 0, 1
         };
};
BLA::Matrix<4, 4> CalcT12(double theta2ins) {
  return {cos(theta2ins - (PI / 2)), -sin(theta2ins - (PI / 2)), 0, 0,
          0,            0, 1, 0,
          sin(theta2ins - (PI / 2)),  cos(theta2ins - (PI / 2)), 0, 0,
          0,            0, 0, 1
         };
};
BLA::Matrix<4, 4> CalcT23(double theta3ins) {
  return {cos(theta3ins), -sin(theta3ins), 0, L1,
          sin(theta3ins),  cos(theta3ins), 0, 0,
          0,            0, 1, 0,
          0,            0, 0, 1
         };
};
BLA::Matrix<4, 4> T3E = {1, 0, 0, L2,
                         0, 1, 0, 0,
                         0, 0, 1, 0,
                         0, 0, 0, 1
                        };


BLA::Matrix<4, 4> TRB0 = TRB * TB0;
//BLA::Matrix<4, 4> TB0_inv = TRB0; //making a matrix for inverse
//bool is_nonsingular = Invert(TB0_inv); //taking the inverse


BLA::Matrix<4, 4> TB0_inv = {0.6428, 0, -0.7660, 274.7309,
                             0, 1, 0, -143.70,
                             0.766, 0, 0.6438, -128.5423,
                             0, 0, 0, 1
                            };

//Function to find the current position from angles
BLA::Matrix<4, 4> GetCurrentPos() { //________________________________________ //________________________________________
  //Total matrix for forward kinematics
  BLA::Matrix<4, 4> TBE = TRB *
                          TB0 *
                          CalcT01(dxl.getPresentPosition(DXL_ID[1], UNIT_DEGREE)) *
                          CalcT12(dxl.getPresentPosition(DXL_ID[2], UNIT_DEGREE)) *
                          //CalcT23(dxl.getPresentPosition(DXL_ID[3], UNIT_DEGREE)) *
                          CalcT23(100) *
                          T3E;

  return TBE;

};


//Function to find neccesary angles to reach desired position
BLA::Matrix<1, 3> setCartesianPosition(double X, double Y, double Z) { //________________________________________
  BLA::Matrix<4, 4> Td = {1, 0, 0, X,
                          0, 1, 0, Y,
                          0, 0, 1, Z,
                          0, 0, 0, 1
                         };

  BLA::Matrix<4, 4> T0E = TB0_inv * Td;

  //Angle of joint 1 (TODO check if this works)(-160 is removed)
  theta1 = atan2(T0E(1, 3), T0E(0, 3));


  //BLA::Matrix<4, 4> T01_inv = CalcT01(theta1); //making a matrix for inverse
  //bool is_nonsingular = Invert(T01_inv); //taking the inverse
  BLA::Matrix<4, 4> T01_inv = {((cos(theta1)) / (cos(theta1)*cos(theta1) + sin(theta1)*sin(theta1))), ((sin(theta1)) / (cos(theta1)*cos(theta1) + sin(theta1)*sin(theta1))), 0, 0,
                               -1 * ((sin(theta1)) / (cos(theta1)*cos(theta1) + sin(theta1)*sin(theta1))), ((cos(theta1)) / (cos(theta1)*cos(theta1) + sin(theta1)*sin(theta1))), 0, 0,
                               0, 0, 1, 0,
                               0, 0, 0, 1
                              };

  //Serial.print("theta1: ");
  //Serial.println(theta1);

  BLA::Matrix<4, 4> T1E = T01_inv * T0E;

  //Serial.print("T01: ");
  //  Serial << CalcT01();

  //Length between joint 1 and end effector grip point
  double L1E = sqrt(pow(T1E(0, 3), 2) + pow(T1E(2, 3), 2));
  //Calculating Phi's
  double phi1 = asin((T1E(2, 3)) / (L1E));
  double phi2 = acos((pow(L1, 2) + pow(L1E, 2) - pow(L2, 2)) / (2 * L1E * L1));
  double phi3 = acos((pow(L1, 2) - pow(L1E, 2) + pow(L2, 2)) / (2 * L2 * L1));

  //Angle of joint 3
  theta3 = PI - phi3;

  //Angle of joint 2
  theta2 = (PI / 2) - (phi1 + phi2);



  theta1 = (theta1 * (180 / PI)) + (BasicValue1 );
  theta2 = (theta2 * (180 / PI)) + (BasicValue2 );
  theta3 = (theta3 * (180 / PI)) + (BasicValue3 );
  //Serial.println();
  //Serial.print("theta1: ");
  //Serial.println(theta1);
  //Serial.print("theta2: ");
  //Serial.println(theta2);
  //Serial.print("theta3: ");
  //Serial.println(theta3);
  //Serial.print("phi1:  ");
  //Serial.println(phi1);
  //Serial.print("phi2:  ");
  //Serial.println(phi2);
  //Serial.print("phi3:  ");
  //Serial.println(phi3);
  //Serial.print("L1E:  ");
  //Serial.println(L1E);

  BLA::Matrix<1, 3> returnvec = {theta1, theta2, theta3};
  return returnvec;
}
