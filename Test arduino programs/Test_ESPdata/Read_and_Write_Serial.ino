boolean readInput(int timeOut) { //insert call function to the other arduino
  byte recieverByte[33];
  signed char recieverChar[33];
  long currentMillis = millis();

  while (true) {
    if (currentMillis + timeOut <= millis()) {
      return 0; //failed communication
      PRINT("Failed")
    }
    if (Serial1.read() == startRByte1) {
      if (Serial1.read() == startRByte2) {
        Serial1.readBytes(recieverByte, 31);

      crc.reset();
      crc.setPolynome(0x07);
      crc.add(startRByte1);
      crc.add(startRByte2);
      for (int i = 0; i < 30; i++)
      {
        recieverChar[i] = recieverByte[i];
        crc.add(recieverByte[i]);
      }

      if (crc.getCRC() == recieverByte[30]) { //should check the last bit
        for (int i = 0; i < 6; i = i + 2) {
            rawThetaref[i / 2] = (recieverChar[i] << 8) + recieverByte[i + 1];
            rawdThetaref[i / 2] = (recieverChar[i + 5] << 8) + recieverByte[i + 6];
            rawddThetaref[i / 2] = (recieverChar[i + 11] << 8) + recieverByte[i + 12];
  
            rawcurTheta[i / 2] = (recieverChar[i + 17] << 8) + recieverByte[i + 18];
            rawcurDTheta[i / 2] = (recieverChar[i + 23] << 8) + recieverByte[i + 24];
          }
        return 1;
      } else return 0;
      }else return 0;
    }else return 0;
  }
}

void writeOutput(int16_t outPut[3]) {
  byte returnValue[9]; //2 start byte + 3 PWM values + CRC

  for (int i = 0; i < 3; i++) {
    returnValue[0] = startByte1;
    returnValue[1] = startByte2;
    returnValue[i * 2 + 2] = highByte(outPut[i]);
    returnValue[i * 2 + 3] = lowByte(outPut[i]);
  }
  crc.reset();
  crc.setPolynome(0x07);
  for (int i = 0; i < 8; i++)
  {
    crc.add(returnValue[i]);
  }
  returnValue[8] = crc.getCRC();
  for (int i = 0; i < 9; i++) {
    soft_serial.write(returnValue[i]);
  }
}
