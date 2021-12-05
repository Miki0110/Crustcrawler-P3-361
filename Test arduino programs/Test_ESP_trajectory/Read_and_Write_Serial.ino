boolean readInput(int timeOut) {
  byte recieverByte[21];
  long currentMillis = millis() + timeOut;

  while (true) {
    if (currentMillis <= millis()) { //small timeout incase no information is given
      return 0; //failed communication
      Serial.println("Failed");
    }
  if (Serial1.read() == startRByte1) { //Check for first start byte
    if (Serial1.read() == startRByte2) { //Check for second start byte
        Serial1.readBytes(recieverByte, 19); //Write the next 19 bytes into reciverByte

      /////////CRC-check////////////////
      crc.reset();
      crc.setPolynome(0x07);
      crc.add(startRByte1);
      crc.add(startRByte2);
      for (int i = 0; i < 18; i++)
      {
        crc.add(recieverByte[i]);
      }
      ///////////////////////////////////
      

      if (crc.getCRC() == recieverByte[18]) { //compare CRC calculated to the reciever byte
        //If successfull save the byte data into variables
        
        for (int i = 0; i < 6; i = i + 2) {
          if (bitRead(recieverByte[i], 0) == 1) { //Incase there's a signed bit indicating a negative value
            Pos_d[i / 2] = (recieverByte[i] << 8) + recieverByte[i + 1];
            bitWrite(Pos_d[i / 2], 17, 0); //Rewritting the byte so it understands it's a signed number and not 32768
          } else {
            Pos_d[i / 2] = (recieverByte[i] << 8) + recieverByte[i + 1]; //If the bit is not signed just write it down
          }

          if (bitRead(recieverByte[i + 6], 0) == 1) { //Incase there's a signed bit indicating a negative value
            rawcurTheta[i / 2] = (recieverByte[i + 6] << 8) + recieverByte[i + 7];
            bitWrite(rawcurTheta[i / 2], 17, 0); //Rewritting the byte so it understands it's a signed number and not 32768
          } else {
            rawcurTheta[i / 2] = (recieverByte[i + 6] << 8) + recieverByte[i + 7]; //If the bit is not signed just write it down
          }

          if (bitRead(recieverByte[i + 12], 0) == 1) { //Incase there's a signed bit indicating a negative value
            rawcurDTheta[i / 2] = (recieverByte[i + 12] << 8) + recieverByte[i + 13];
            bitWrite(rawcurDTheta[i / 2], 17, 0); //Rewritting the byte so it understands it's a signed number and not 32768
          } else {
            rawcurDTheta[i / 2] = (recieverByte[i + 12] << 8) + recieverByte[i + 13]; //If the bit is not signed just write it down
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
