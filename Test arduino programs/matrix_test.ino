#include <BasicLinearAlgebra.h>
#include <math.h>

using namespace BLA;

double Lb = 100;
double L1 = 219.8; 
double L2 = 147.3+85.5;
Matrix<4,4> Td = {1, 0, 0, Lb, 
                  0, 1, 0, Lb,
                  0, 0, 1, Lb,
                  0, 0, 0, 1};

void setup()
{
  Serial.begin(9600);
  
    Matrix<4,4> TB0 = {1, 0, 0, 0, 
                       0, 1, 0, 0,
                       0, 0, 1, Lb,
                       0, 0, 0, 1};
    double theta1=atan2(Td(1,3),Td(0,3));
    Serial.println(theta1*180/PI);
    Matrix<4,4> T01 = {cos(theta1), -sin(theta1), 0, 0, 
                       sin(theta1),  cos(theta1), 0, 0,
                       0,            0,           1, 0,
                       0,            0,           0, 1};
   Matrix<4,4> TB1 = TB0*T01;    
   Matrix<4,4> TB1_inv = TB1; //making a matrix for inverse
   bool is_nonsingular = Invert(TB1_inv); //taking the inverse
    
    Matrix<4,4> T1E = TB1_inv * Td;
    Serial << "T1E: " << T1E << "\n";

   double phi1 = atan2(T1E(2,3),T1E(0,3));
    Serial.println(T1E(2,3));
    Serial.println(T1E(0,3));
   double L1E = sqrt(pow(T1E(0,3),2)+pow(T1E(2,3),2));
   Serial.println(L1E);
   double phi2 = acos((pow(L1,2)+pow(L1E,2)-pow(L2,2))/(2*L1E*L1));
   Serial.println(phi2);
   double phi3 = acos((pow(L1E,2)-pow(L1,2)-pow(L2,2))/(2*L1*L2));

  double theta2=2*PI-(-phi1-phi2+PI/2);
  double theta3=PI-phi3;

 Matrix<3> theta={theta1,theta2,theta3};
  Matrix<3> theta_deg=theta*180/PI;
  Serial << "theta1: " << theta(0) << "\n";
  Serial << "theta2: " << theta(1) << "\n";
  Serial << "theta3: " << theta(2) << "\n";
  Serial << "theta1 deg: " << theta_deg(0) << "\n";
  Serial << "theta2 deg: " << theta_deg(1) << "\n";
  Serial << "theta3 deg: " << theta_deg(2) << "\n";
}

void loop() {}
