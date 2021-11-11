//Link lengths
const double Lrx = 102.98;//X coordinate reference to joint 1 motor (base)
const double Lry = 151.40;//Y coordinate reference to joint 1 motor (base)
const double Lrz = 228.14;//Z coordinate reference to joint 1 motor (base)
const double Lra = -45.00;//Base tilt angle compared to reference (in deg)

const double Lb =  59.41; //Base link (motor 2)
const double L1 = 219.80; //Link 1
const double L2 = 147.30; //Link 2
const double Lg = 145.14; //From gripper joints to grip point

double theta1;
double theta2;
double theta3;

//4x4 Homogenous transformation matricies from crustcrawler
BLA::Matrix<4,4> TRB = { cos(Lra*PI/180), 0, sin(Lra*PI/180),  Lrx,
                                       0, 1,               0,  Lry,
                        -sin(Lra*PI/180), 0, cos(Lra*PI/180),  Lrz,
                                       0, 0,               0,  1};
BLA::Matrix<4,4> TB0 = {1, 0, 0, 0, 
                        0, 1, 0, 0,
                        0, 0, 1, Lb,
                        0, 0, 0, 1};
BLA::Matrix<4,4> T01 = {cos(theta1), -sin(theta1), 0, 0, 
                        sin(theta1),  cos(theta1), 0, 0,
                                  0,            0, 1, 0,
                                  0,            0, 0, 1};
BLA::Matrix<4,4> T12 = {cos(theta2), -sin(theta2), 0, 0, 
                                  0,            0, 0, 0,
                        sin(theta2),  cos(theta2), 1, 0,
                                  0,            0, 0, 1};
BLA::Matrix<4,4> T23 = {cos(theta3), -sin(theta3), 0, L1, 
                        sin(theta3),  cos(theta3), 0, 0,
                                  0,            0, 1, 0,
                                  0,            0, 0, 1};
BLA::Matrix<4,4> T3E = {1, 0, 0, L2+Lg, 
                        0, 1, 0, 0,
                        0, 0, 1, 0,
                        0, 0, 0, 1};

//Total matrix for forward kinematics
BLA::Matrix<4,4> TBE = TRB * TB0 * T01 * T12 * T23 * T3E;

BLA::Matrix<4,4> TB1 = TB0 * T01;
BLA::Matrix<4,4> TB1_inv = TB1; //making a matrix for inverse
bool is_nonsingular = Invert(TB1_inv); //taking the inverse

//Function to find neccesary angles to reach desired position
boolean setCartesianPosition(double X, double Y, double Z) {
  BLA::Matrix<4,4> Td = {1, 0, 0, X, 
                         0, 1, 0, Y,
                         0, 0, 1, Z,
                         0, 0, 0, 1};

  BLA::Matrix<4,4> T1E = TB1_inv * Td;

  //Angle of joint 1 (TODO check if this works) (the -160 is due to reference alignment)
  theta1 = atan2(Td(1,3) - 160, Td(0,3));

  //Length between joint 1 and end effector grip point
  double L1E = sqrt(pow(T1E(0,3),2)+pow(T1E(1,3),2)+pow(T1E(2,3),2));

  //Angle of joint 3
  theta3 = acos((-pow(L1E,2)-pow(L1,2)-pow(L2,2))/(2*L1*L2));

  //Angle of joint 2
  double phi1 = atan2(T1E(2,3),sqrt(pow(T1E(0,3),2)+pow(T1E(1,3),2)));
  double phi2 = acos((pow(L1,2)+pow(L1E,2)-pow(L2,2))/(2*L1E*L1));
  theta2 = PI+phi1+phi2; 
}
