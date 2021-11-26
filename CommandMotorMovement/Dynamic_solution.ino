/*/////////////////////////////////////////////////////////////////////////////////////////////////
                    Holy shit guys don't put delays in this(or anything for that matter)
  /////////////////////////////////////////////////////////////////////////////////////////////////*/

bool callPWM(int Thetaref[3], int dThetaref[3], int ddThetaref[3]) {

  byte startByte = 0x5A
                   int currTheta[3], currDThteta[3];
  //Read current positions
  for (int i = 0; i < 3; i++) {
    currTheta[i] = dxl.getPresentPosition(id, UNIT_RAW);
    currDTheta[i] = dxl.getPresentVelocity(id, UNIT_RAW);
  }

  //convert to bytes and write
  soft_serial.write(startByte);
  for (int i = 0; i < 3; i++) {
    soft_serial.write(highByte(Thetaref[i]));
    soft_serial.write(lowByte(Thetaref[i]));
  }
  for (int i = 0; i < 3; i++) {
    soft_serial.write(highByte(dThetaref[i]));
    soft_serial.write(lowByte(dThetaref[i]));
  }
  for (int i = 0; i < 3; i++) {
    soft_serial.write(highByte(ddThetaref[i]));
    soft_serial.write(lowByte(ddThetaref[i]));
  }
  for (int i = 0; i < 3; i++) {
    soft_serial.write(highByte(currTheta[i]));
    soft_serial.write(lowByte(currTheta[i]);
  }
  for (int i = 0; i < 3; i++) {
    soft_serial.write(highByte(currDTheta[i]));
    soft_serial.write(lowByte(currDTheta[i]);
  }
  
  //Wait and read answer
  byte recieverByte[7];
  long currentMillis = millis();

  int PWMvalue[3];

  while(Serial.read() != startRByte){
    if(currentMillis + 200 <= millis()){
      return 0;}
    }
      Serial.readBytes(recieverByte, 6);

        for (int i = 0; i < 6; i = i + 2) {
          PWMvalue[i / 2] = (recieverByte[i] << 8) + recieverByte[i + 1];
          PWMvalue[i / 2] = (recieverByte[i + 6] << 8) + recieverByte[i + 7];
        }
        for (int i = 0; i < 3; i++){
        setPWM(DXL_ID[i+1], PWMvalue[i]);
        }
        return 1;

  }
}
 
