//double a = 8.583 * 254 * (speed_mod / 5); //max acceleration for the MX-64
BLA::Matrix<1, 3> cubicPolyAll(unsigned long t_ms, float theta0, float thetaf, unsigned long tf_ms) { //function for the cubic polynomial (All)
  float a0 = theta0;
  float a1 = 0;
  float a2 = 3 / pow(tf_ms,2) * (thetaf - theta0);
  float a3 = -(2 /pow(tf_ms,3)) * (thetaf - theta0);
  BLA::Matrix<1, 3> ReturnAll = {a0 + a1 * t_ms + a2 * pow(t_ms,2) + a3 *pow(t_ms,3), //Pos
                                 a1 + 2 * a2 * t_ms + 3 * a3 * pow(t_ms,2), //Vel
                                 2 * a2 + 6 * a3 * t_ms //Acc
                                };
  return ReturnAll;//Pos, Vel, Acc
} //End of cubicPolyAll()
