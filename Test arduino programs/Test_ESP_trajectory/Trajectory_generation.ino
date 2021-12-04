double speed_mod = 0.03;
double max_vel = (0.114 * 1023) * 6 * speed_mod;
//double a = 8.583 * 254 * (speed_mod / 5); //max acceleration for the MX-64


BLA::Matrix<1, 3> cubicPolyAll(double t, double theta0, double thetaf, double max_vel, double tf) { //function for the cubic polynomial (All)
  double a0 = theta0;
  double a1 = 0;
  double a2 = 3 / (tf * tf) * (thetaf - theta0);
  double a3 = -(2 / (tf * tf * tf)) * (thetaf - theta0);
  BLA::Matrix<1, 3> ReturnAll = {a0 + a1 * t + a2 * t * t + a3 * t * t * t, //Pos
                                 a1 + 2 * a2 * t + 3 * a3 * t * t, //Vel
                                 2 * a2 + 6 * a3 * t //Acc
                                }; 
  return ReturnAll;//Pos, Vel, Acc
}//End of cubicPolyAll()

double cubicPolyPos(double t, double theta0, double thetaf, double max_vel, double tf) { //function for the cubic polynomial (Position)
  double a0 = theta0;
  double a1 = 0;
  double a2 = 3 / (tf * tf) * (thetaf - theta0);
  double a3 = -(2 / (tf * tf * tf)) * (thetaf - theta0);

  return a0 + a1 * t + a2 * t * t + a3 * t * t * t;
}//End of cubicPolyPos()

double cubicPolyVel(double t, double theta0, double thetaf, double max_vel, double tf) { //function for the cubic polynomial (Velocity)
  double a0 = theta0;
  double a1 = 0;
  double a2 = 3 / (tf * tf) * (thetaf - theta0);
  double a3 = -(2 / (tf * tf * tf)) * (thetaf - theta0);

  return a1 + 2 * a2 * t + 3 * a3 * t * t;
}//End of cubicPolyVel()

double cubicPolyAcc(double t, double theta0, double thetaf, double max_vel, double tf) { //function for the cubic polynomial (Acceleration)
  double a0 = theta0;
  double a1 = 0;
  double a2 = 3 / (tf * tf) * (thetaf - theta0);
  double a3 = -(2 / (tf * tf * tf)) * (thetaf - theta0);

  return 2 * a2 + 6 * a3 * t;
}//End of cubicPolyAcc)(

BLA::Matrix<3, 3> TrajectoryGeneration(double newx, double newy, double newz, double curTheta[3]) {
  //Initialicing variables and numbers
  int NewPosCartesian[] = {newx, newy, newz};
  BLA::Matrix<1, 3> NewPosAngels = {setCartesianPosition(newx, newy, newz)};
  BLA::Matrix<4, 4> OldPosCartesianMatrix = GetCurrentPos(curTheta);
  int OldPosCartesian[] = {OldPosCartesianMatrix(0, 3), OldPosCartesianMatrix(1, 3), OldPosCartesianMatrix(2, 3)};

  BLA::Matrix<3, 3> GeneratedTrajectory = {}; //The return matrix

  return GeneratedTrajectory;
} //End of TrajectoryGeneration (Function)
