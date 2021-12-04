boolean readInput(int timeOut) { //insert call function to the other arduino
  byte recieverByte[33];
  long currentMillis = millis();

  //int rawThetaref[3], rawdThetaref[3], rawddThetaref[3], rawcurTheta[3], rawcurDTheta[3];

  while (true) {
    if (currentMillis + timeOut <= millis()) {
      return 0; //failed communication
      Serial.println("Failed");
    }
    if (Serial1.read() == startRByte1) {
      if (Serial1.read() == startRByte2) {
        Serial1.readBytes(recieverByte, 31);

      crc.reset();
      crc.setPolynome(0x07);
      crc.add(startRByte1);
      crc.add(startRByte2);
//      Serial.print("byte recived: ");
//      Serial.print(startRByte, HEX);
      for (int i = 0; i < 30; i++)
      {
//        Serial.print(recieverByte[i], HEX);
        crc.add(recieverByte[i]);
      }
//      Serial.print(crc.getCRC(), HEX);
//      Serial.println("");

      if (crc.getCRC() == recieverByte[30]) { //should check the last bit
        for (int i = 0; i < 6; i = i + 2) {
          if (bitRead(recieverByte[i], 0) == 1) {
            bitWrite(recieverByte[i], 0, 0);
            rawThetaref[i / 2] = -(recieverByte[i] << 8) + recieverByte[i + 1];
          } else {
            rawThetaref[i / 2] = (recieverByte[i] << 8) + recieverByte[i + 1];
          }

          if (bitRead(recieverByte[i + 5], 0) == 1) {
            bitWrite(recieverByte[i + 5], 0, 0);
            rawdThetaref[i / 2] = -(recieverByte[i + 5] << 8) + recieverByte[i + 6];
          } else {
            rawdThetaref[i / 2] = (recieverByte[i + 5] << 8) + recieverByte[i + 6];
          }

          if (bitRead(recieverByte[i + 11], 0) == 1) {
            bitWrite(recieverByte[i + 11], 0, 0);
            rawThetaref[i / 2] = -(recieverByte[i + 11] << 8) + recieverByte[i + 12];
          } else {
            rawThetaref[i / 2] = (recieverByte[i + 11] << 8) + recieverByte[i + 12];
          }

          if (bitRead(recieverByte[i + 17], 7) == 1) {
            //bitWrite(recieverByte[i + 17], 7, 0);
            rawcurTheta[i / 2] = (recieverByte[i + 17] << 8) + recieverByte[i + 18];
            bitWrite(rawcurTheta[i / 2], 17, 0);
          } else {
            rawcurTheta[i / 2] = (recieverByte[i + 17] << 8) + recieverByte[i + 18];
          }

          if (bitRead(recieverByte[i + 23], 7) == 1) {
            rawcurDTheta[i / 2] = (recieverByte[i + 23] << 8) + recieverByte[i + 24];
            bitWrite(rawcurTheta[i / 2], 17, 0);
          } else {
            rawcurDTheta[i / 2] = (recieverByte[i + 23] << 8) + recieverByte[i + 24];
          }

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
    //Serial.print(returnValue[i], HEX);
    soft_serial.write(returnValue[i]);
  }
  //Serial.println("");
}
