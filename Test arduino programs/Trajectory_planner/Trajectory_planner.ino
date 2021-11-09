#include <BasicLinearAlgebra.h>
#include <math.h>


//keep in mind the function might need changes to also get the desired velocity and acceleration

using namespace BLA;

  //Input needed to call the function
  double speed_mod=0.03;
  double max_vel=(0.114*1023)*6*speed_mod;
  Matrix<1,3> theta0 = {10,10,10}; //start position
  Matrix<1,3> thetaf = {60,80,-50}; //end position

 //valuables calculated at the start
  double starttime = millis();
  double tf = sqrt(pow((theta0(0,0)-thetaf(0,0)),2)+pow((theta0(0,1)-thetaf(0,1)),2)+pow((theta0(0,2)-thetaf(0,2)),2))/(max_vel);
  double a=8.583*254*(speed_mod/5); //max acceleration for the MX-64
void setup() {
  // put your setup code here, to run once:
  
  Serial.begin(9600); //For de-bugging
  //Serial.println(max_vel);
  //Serial.println(a);
  //Serial.print("time to finish: ");
  //Serial.println(tf);
}

void loop() {
  // put your main code here, to run repeatedly:
  double t=(millis()-starttime)/1000; //just to simulate time
  //Serial.print("current time: ");
  //Serial.println(t);
  if(t<tf){
double cTheta1 = cubicPoly(t,theta0(0,0),thetaf(0,0),max_vel,tf);
double cTheta2 = cubicPoly(t,theta0(0,1),thetaf(0,1),max_vel,tf);
double cTheta3 = cubicPoly(t,theta0(0,2),thetaf(0,2),max_vel,tf);

double pTheta1 = paraBlend(t,theta0(0,0),thetaf(0,0),max_vel,a,tf);
double pTheta2 = paraBlend(t,theta0(0,1),thetaf(0,1),max_vel,a,tf);
double pTheta3 = paraBlend(t,theta0(0,2),thetaf(0,2),max_vel,a,tf);

/////////////Debug////////////////
/*
Serial.print("Cube_Theta1 ");
Serial.print(cTheta1);
Serial.print(" ");
Serial.print("Cube_Theta2 ");
Serial.print(cTheta2);
Serial.print(" ");
Serial.print("Cube_Theta3 ");
//Serial.print("Para Theta1 ");
Serial.print(cTheta3);
//Serial.print(" ");
*/
Serial.print("Para_Theta1 ");
Serial.print(pTheta1);
Serial.print(" ");
Serial.print("Para_Theta2 ");
Serial.print(pTheta2);
Serial.print(" ");
Serial.print("Para_Theta3 ");
Serial.println(pTheta3);
Serial.print(" ");
//////////////////////////////////////
  }
}

double cubicPoly(double t,double theta0,double thetaf,double max_vel,double tf){ //function for the cubic polynomial
  double a0 = theta0;
  double a1 = 0;
  double a2 = 3/(tf*tf)*(thetaf-theta0);
  double a3 = -(2/(tf*tf*tf))*(thetaf-theta0);
 
  return a0+a1*t+a2*t*t+a3*t*t*t;
  //double vt(t)=a1+2*a2*t+3*a3*t^2;
  //double at(t)=2*a2+6*a3*t;
  }
 double paraBlend(double t,double theta0,double thetaf,double max_vel,double a,double tf){ //function for the parabolic blend
  double tb,theta0b,thetabt,thetatf = 0.0;
  if(theta0<thetaf){
     tb = tf/2 - sqrt((a*a)*(tf*tf)-4*a*(thetaf-theta0))/(2*a);
     double thetab=0.5*a*tb*tb+theta0;
     theta0b= theta0+0.5*a*(t*t);
     thetabt= thetab+a*tb*(t-tb);
     thetatf= thetaf-0.5*a*pow((tf-t),2);
  }else{
     tb = tf/2 - sqrt((a*a)*(tf*tf)-4*a*(theta0-thetaf))/(2*a);
     double thetab = theta0-0.5*a*tb*tb;
     theta0b = theta0-0.5*a*(t*t);
     thetabt = thetab-a*tb*(t-tb);
     thetatf = thetaf+0.5*a*pow((tf-t),2);
  }
  if(t<tb){
  return theta0b;}
  else if(t<tf-tb){
  return thetabt;}
  else if(tf-tb<!t<tf){
  return thetatf;}
  }
