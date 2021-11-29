/*/////////////////////////////////////////////////////////////////////////////////////////////////
                    Holy shit guys don't put delays in this(or anything for that matter)
  /////////////////////////////////////////////////////////////////////////////////////////////////*/

bool callPWM(int Thetaref[3], int dThetaref[3], int ddThetaref[3]) {

//////////////////////////////////////////////////////////////////////////////////////////////
////                            write all the data and send it                          //////
  byte startByte = 0x5A; //send byte
  byte startRByte = 0xDF; //reciver byte
  byte positionMessage[32];
                   int currTheta[3], currDThteta[3];
  //Read current positions
  for (int i = 0; i < 3; i++) {
    currTheta[i] = dxl.getPresentPosition(id, UNIT_RAW);
    currDTheta[i] = dxl.getPresentVelocity(id, UNIT_RAW);
  }

  //convert to bytes and write
  positionMessage[0]=startByte;
  for (int i = 0; i < 3; i++) {
    positionMessage[i*2+1]=highByte(Thetaref[i]);
    positionMessage[i*2+2]=lowByte(Thetaref[i]);
  }
  for (int i = 0; i < 3; i++) {
    positionMessage[i*2+6]=highByte(dThetaref[i]);
    positionMessage[i*2+7]=lowByte(dThetaref[i]);
  }
  for (int i = 0; i < 3; i++) {
    positionMessage[i*2+12]=highByte(ddThetaref[i]);
    positionMessage[i*2+13]=lowByte(ddThetaref[i]);
  }
  for (int i = 0; i < 3; i++) {
    positionMessage[i*2+18]=highByte(currTheta[i]);
    positionMessage[i*2+19]=lowByte(currTheta[i]);
  }
  for (int i = 0; i < 3; i++) {
    positionMessage[i*2+24]=highByte(currDTheta[i]);
    positionMessage[i*2+25]=lowByte(currDTheta[i]);
  }
  //add the CRC-check
  crc.reset();
  crc.setPolynome(0x07);
  for (int i = 0; i < 31; i++)
  {
    crc.add(positionMessage[i]);
  }
    positionMessage[31]=crc.getCRC();
    soft_serial.write(positionMessage);
////                                                                                    //////
//////////////////////////////////////////////////////////////////////////////////////////////

  //Wait and read answer
  byte recieverByte[8];
  long currentMillis = millis();

  int PWMvalue[3];

  while (Serial.read() != startRByte) {
    if (currentMillis + 200 <= millis()) {
      return 0;
    }
  }
  Serial.readBytes(recieverByte, 7);
  
      crc.reset();
      crc.setPolynome(0x07);
      crc.add(startRByte);
      for (int i = 0; i < 6; i++)
      {
        crc.add(recieverByte[i]);
      }
      Serial.print("CRC calced: ");
      Serial.println(crc.getCRC(), HEX);

      Serial.print("Last byte: ");
      Serial.println(recieverByte[6], HEX);
 if (crc.getCRC() == recieverByte[6]) {     
  for (int i = 0; i < 7; i = i + 2) {
    PWMvalue[i / 2] = (recieverByte[i] << 8) + recieverByte[i + 1];
    PWMvalue[i / 2] = (recieverByte[i + 6] << 8) + recieverByte[i + 7];
  }
  for (int i = 0; i < 3; i++) {
    setPWM(DXL_ID[i + 1], PWMvalue[i]);
  }
  
  return 1;
 }
}
}
