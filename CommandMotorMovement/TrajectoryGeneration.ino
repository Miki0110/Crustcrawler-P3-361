
BLA::Matrix<1, 3> TrajectoryGeneration(int newx, int newy, int newz) {
  //Initialicing variables and numbers
  int NewPosCartesian[] = {newx, newy, newz};
  BLA::Matrix<1, 3> NewPosAngels[] = {setCartesianPosition(newx, newy, newz)};
  int OldPosAngels[] = {dxl.getPresentPosition(DXL_ID[1], UNIT_DEGREE), dxl.getPresentPosition(DXL_ID[2], UNIT_DEGREE), dxl.getPresentPosition(DXL_ID[3], UNIT_DEGREE)};
  BLA::Matrix<4, 4> OldPosCartesianMatrix = GetCurrentPos();
  int OldPosCartesian[] = {OldPosCartesianMatrix(0, 3), OldPosCartesianMatrix(1, 3), OldPosCartesianMatrix(2, 3)};

  BLA::Matrix<1, 3> GeneratedTrajectory = {}; //The return matrix

  for (int i = 1; i <= 3; i++) {
    //Missing centerpart.
    //Something to calculate the Trajectory by using Varibles above


    GeneratedTrajectory(0, i - 1) = 0;
  }



  return GeneratedTrajectory;
} //End of TrajectoryGeneration (Function)


BLA::Matrix<1, 3> cubicPoly(double t, double theta0, double thetaf, double max_vel, double tf) { //function for the cubic polynomial (All)
  double a0 = theta0;
  double a1 = 0;
  double a2 = 3 / (tf * tf) * (thetaf - theta0);
  double a3 = -(2 / (tf * tf * tf)) * (thetaf - theta0);
  BLA::Matrix<1, 3> ReturnAll = {a0 + a1 * t + a2 * t * t + a3 * t * t * t, //Pos
                                 a1 + 2 * a2 * t + 3 * a3 * t * t, //Vel
                                 2 * a2 + 6 * a3 * t //Acc
                                }; //Pos, Vel, Acc

  return ReturnAll;
}

double cubicPolyPos(double t, double theta0, double thetaf, double max_vel, double tf) { //function for the cubic polynomial (Position)
  double a0 = theta0;
  double a1 = 0;
  double a2 = 3 / (tf * tf) * (thetaf - theta0);
  double a3 = -(2 / (tf * tf * tf)) * (thetaf - theta0);

  return a0 + a1 * t + a2 * t * t + a3 * t * t * t;
}

double cubicPolyVel(double t, double theta0, double thetaf, double max_vel, double tf) { //function for the cubic polynomial (Velocity)
  double a0 = theta0;
  double a1 = 0;
  double a2 = 3 / (tf * tf) * (thetaf - theta0);
  double a3 = -(2 / (tf * tf * tf)) * (thetaf - theta0);

  return a1 + 2 * a2 * t + 3 * a3 * t * t;
}

double cubicPolyAcc(double t, double theta0, double thetaf, double max_vel, double tf) { //function for the cubic polynomial (Acceleration)
  double a0 = theta0;
  double a1 = 0;
  double a2 = 3 / (tf * tf) * (thetaf - theta0);
  double a3 = -(2 / (tf * tf * tf)) * (thetaf - theta0);

  return 2 * a2 + 6 * a3 * t;
}

double paraBlend(double t, double theta0, double thetaf, double max_vel, double a, double tf) { //function for the parabolic blend
  double tb, theta0b, thetabt, thetatf = 0.0;
  tb = tf / 2 - sqrt((a * a) * (tf * tf) - 4 * a * (thetaf - theta0)) / (2 * a);
  if (theta0 < thetaf) {
    double thetab = 0.5 * a * tb * tb + theta0;
    theta0b = theta0 + 0.5 * a * (t * t);
    thetabt = thetab + a * tb * (t - tb);
    thetatf = thetaf - 0.5 * a * pow((tf - t), 2);
  } else {
    double thetab = theta0 - 0.5 * a * tb * tb;
    theta0b = theta0 - 0.5 * a * (t * t);
    thetabt = thetab - a * tb * (t - tb);
    thetatf = thetaf + 0.5 * a * pow((tf - t), 2);
  }
  if (t < tb) {
    return theta0b;
  }
  else if (t < tf - tb) {
    return thetabt;
  }
  else if (tf - tb < !t < tf) {
    return thetatf;
  }
} //End of ParaBlend (Function)
