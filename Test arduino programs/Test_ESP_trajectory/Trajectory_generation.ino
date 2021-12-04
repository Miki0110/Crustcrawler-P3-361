//double a = 8.583 * 254 * (speed_mod / 5); //max acceleration for the MX-64


BLA::Matrix<1, 3> cubicPolyAll(float t, float theta0, float thetaf, float max_vel, float tf) { //function for the cubic polynomial (All)
  float a0 = theta0;
  float a1 = 0;
  float a2 = 3 / (tf * tf) * (thetaf - theta0);
  float a3 = -(2 / (tf * tf * tf)) * (thetaf - theta0);
  BLA::Matrix<1, 3> ReturnAll = {a0 + a1 * t + a2 * t * t + a3 * t * t * t, //Pos
                                 a1 + 2 * a2 * t + 3 * a3 * t * t, //Vel
                                 2 * a2 + 6 * a3 * t //Acc
                                }; 
  return ReturnAll;//Pos, Vel, Acc
}//End of cubicPolyAll()
