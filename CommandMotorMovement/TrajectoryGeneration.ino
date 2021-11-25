double speed_mod = 0.03;
double max_vel = (0.114 * 1023) * 6 * speed_mod;
double a = 8.583 * 254 * (speed_mod / 5); //max acceleration for the MX-64


BLA::Matrix<1, 3> cubicPolyAll(double t, double theta0, double thetaf, double max_vel, double tf) { //function for the cubic polynomial (All)
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

BLA::Matrix<3, 3> TrajectoryGeneration(double newx, double newy, double newz) {
  //Initialicing variables and numbers
  int NewPosCartesian[] = {newx, newy, newz};
  BLA::Matrix<1, 3> NewPosAngels = {setCartesianPosition(newx, newy, newz)};
  int OldPosAngels[] = {dxl.getPresentPosition(DXL_ID[1], UNIT_DEGREE), dxl.getPresentPosition(DXL_ID[2], UNIT_DEGREE), dxl.getPresentPosition(DXL_ID[3], UNIT_DEGREE)};
  BLA::Matrix<4, 4> OldPosCartesianMatrix = GetCurrentPos();
  int OldPosCartesian[] = {OldPosCartesianMatrix(0, 3), OldPosCartesianMatrix(1, 3), OldPosCartesianMatrix(2, 3)};
  double tf = sqrt(pow((OldPosAngels[0] - NewPosAngels(0, 0)), 2) + pow((OldPosAngels[1] - NewPosAngels(0, 1)), 2) + pow((OldPosAngels[2] - NewPosAngels(0, 2)), 2)) / (max_vel);

  BLA::Matrix<3, 3> GeneratedTrajectory = {}; //The return matrix
  Serial.println();
  Serial.println("The GeneratedTrajectory for Pos is :");
  Serial.println("Pos    Vel    Acc");
  for (int i = 1; i <= 3; i++) {
    BLA::Matrix<1, 3> Calculated = cubicPolyAll(10, OldPosAngels[i - 1], NewPosAngels(0, i - 1), max_vel, tf);
    GeneratedTrajectory(i - 1, 0) = Calculated(0, 0); //Taking the Pos
    GeneratedTrajectory(i - 1, 1) = Calculated(0, 1); //Taking the Vel
    GeneratedTrajectory(i - 1, 2) = Calculated(0, 2); //Taking the Acc
    for (int j = 1; j <= 3; j++) {
      Serial.print(GeneratedTrajectory(i, j - 1));
      Serial.print("   ");
    }
    Serial.println();
  }
  return GeneratedTrajectory;
} //End of TrajectoryGeneration (Function)
