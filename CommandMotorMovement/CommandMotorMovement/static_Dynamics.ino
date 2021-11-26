double computeStaticTorque(int id, float theta1, float theta2, float theta3) {
  //thetas should be in rads
  double tau;
  switch(id){
  case 1:
    Serial.println("HERE  1!!!");
    tau = 0.0008774170000*sin(PI*0.25 + theta1) + 0.0008774170000*sin(PI*0.25 - theta1) + 0.1068143495*cos(PI*0.25 + theta1 - theta2 - theta3) - 0.1068143495*cos((3*PI)*0.25 + theta1 - theta2 - theta3) + 0.1068143495*cos((3*PI)*0.25 - theta1 - theta2 - theta3) - 0.1068143495*cos(PI*0.25 - theta1 - theta2 - theta3) + 0.2714675170*cos(PI*0.25 + theta1 - theta2) + 0.2714675170*cos((3*PI)*0.25 - theta1 - theta2) - 0.2714675170*cos(PI*0.25 - theta1 - theta2) - 0.2714675170*cos((3*PI)*0.25 + theta1 - theta2) - 0.0002288060000*sin(PI*0.25 - theta1 - theta2) + 0.0002288060000*sin(PI*0.25 + theta1 - theta2) - 0.0002288060000*sin((3*PI)*0.25 + theta1 - theta2) + 0.0004261880000*cos(PI*0.25 - theta1) - 0.0004261880000*cos(PI*0.25 + theta1) + 0.0002288060000*sin((3*PI)*0.25 - theta1 - theta2);
    return tau;
    break;
  case 2:
    tau = 0.5429350340*cos(-theta2 + PI*0.25) - 0.0004576120000*sin(-theta2 + (3*PI)*0.25) + 0.2136286990*cos(theta3 + (3*PI)*0.25 - theta2) - 0.1068143495*cos(PI*0.25 + theta1 - theta2 - theta3) + 0.1068143495*cos((3*PI)*0.25 + theta1 - theta2 - theta3) + 0.1068143495*cos((3*PI)*0.25 - theta1 - theta2 - theta3) - 0.1068143495*cos(PI*0.25 - theta1 - theta2 - theta3) - 0.2714675170*cos(PI*0.25 + theta1 - theta2) + 0.2714675170*cos((3*PI)*0.25 - theta1 - theta2) - 0.2714675170*cos(PI*0.25 - theta1 - theta2) + 0.2714675170*cos((3*PI)*0.25 + theta1 - theta2) - 0.0002288060000*sin(PI*0.25 - theta1 - theta2) - 0.0002288060000*sin(PI*0.25 + theta1 - theta2) + 0.0002288060000*sin((3*PI)*0.25 + theta1 - theta2) + 0.0002288060000*sin((3*PI)*0.25 - theta1 - theta2) - 0.0004576120000*sin(-theta2 + PI*0.25) + 0.5429350340*cos(-theta2 + (3*PI)*0.25) + 0.2136286990*cos(theta3 + PI*0.25 - theta2);
    Serial.println("HERE  2!!!");
    return tau;
    break;
  case 3:
   tau = -0.2136286990*cos(theta3 + (3*PI)*0.25 - theta2) - 0.1068143495*cos(PI*0.25 + theta1 - theta2 - theta3) + 0.1068143495*cos((3*PI)*0.25 + theta1 - theta2 - theta3) + 0.1068143495*cos((3*PI)*0.25 - theta1 - theta2 - theta3) - 0.1068143495*cos(PI*0.25 - theta1 - theta2 - theta3) - 0.2136286990*cos(theta3 + PI*0.25 - theta2);
   Serial.println("HERE  3!!!");
   return tau;
  break;
  default:
  Serial.println("fuck!!!");
    return 0.0;
    break;
    }
  }
