//Link lengths
double Lb =  13.80; //Top of baseplate to joint 1
double L1 =  59.41; //Link 1
double L2 = 219.80; //Link 2
double L3 = 147.30; //Link 3
double Lg = 145.14; //From gripper joints to grip point

double theta1;
double theta2;
double theta3;

//4x4 Homogenous transformation matricies from forward kinematics
BLA::Matrix<4,4> TB0 = {1, 0, 0, 0, 
                   0, 1, 0, 0,
                   0, 0, 1, Lb+L1,
                   0, 0, 0, 1};
BLA::Matrix<4,4> T01 = {cos(theta1), -sin(theta1), 0, 0, 
                        sin(theta1),  cos(theta1), 0, 0,
                                  0,            0, 1, 0,
                                  0,            0, 0, 1};
BLA::Matrix<4,4> T12 = {cos(theta2), -sin(theta2), 0, 0, 
                                  0,            0, 0, 0,
                        sin(theta2),  cos(theta2), 1, 0,
                                  0,            0, 0, 1};
BLA::Matrix<4,4> T23 = {cos(theta3), -sin(theta3), 0, L2, 
                        sin(theta3),  cos(theta3), 0, 0,
                                  0,            0, 1, 0,
                                  0,            0, 0, 1};
BLA::Matrix<4,4> T3E = {1, 0, 0, L3+Lg, 
                        0, 1, 0, 0,
                        0, 0, 1, 0,
                        0, 0, 0, 1};
BLA::Matrix<4,4> TBE = TB0 * T01 * T12 * T23 * T3E;
BLA::Matrix<4,4> T1E = T12 * T23 * T3E;

boolean setCartesianPosition(double X, double Y, double Z) {
  
}
