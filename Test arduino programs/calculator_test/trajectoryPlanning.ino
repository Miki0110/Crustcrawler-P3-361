double cubicPolyPos(double t,double theta0,double thetaf,double max_vel,double tf){ //function for the cubic polynomial
  double a0 = theta0;
  double a1 = 0;
  double a2 = 3/(tf*tf)*(thetaf-theta0);
  double a3 = -(2/(tf*tf*tf))*(thetaf-theta0);
 
  return a0+a1*t+a2*t*t+a3*t*t*t;
}

double cubicPolyVel(double t,double theta0,double thetaf,double max_vel,double tf){ //function for the cubic polynomial
  double a0 = theta0;
  double a1 = 0;
  double a2 = 3/(tf*tf)*(thetaf-theta0);
  double a3 = -(2/(tf*tf*tf))*(thetaf-theta0);
 
  return a1+2*a2*t+3*a3*t*t;
}

double cubicPolyAcc(double t,double theta0,double thetaf,double max_vel,double tf){ //function for the cubic polynomial
  double a0 = theta0;
  double a1 = 0;
  double a2 = 3/(tf*tf)*(thetaf-theta0);
  double a3 = -(2/(tf*tf*tf))*(thetaf-theta0);
 
  return 2*a2+6*a3*t;
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
