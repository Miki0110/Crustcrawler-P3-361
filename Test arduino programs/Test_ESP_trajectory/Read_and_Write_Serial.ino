boolean readInput(int timeOut) {
  byte recieverByte[21];
  signed char recieverChar[21];
  long currentMillis = millis() + timeOut;

  while (true) {
    if (currentMillis <= millis()) { //small timeout incase no information is given
      return 0; //failed communication
      PRINT("Failed");
    }
  if (Serial1.read() == startRByte1) { //Check for first start byte
    if (Serial1.read() == startRByte2) { //Check for second start byte
        Serial1.readBytes(recieverByte, 19); //Write the next 19 bytes into reciverByte

      /////////CRC-check////////////////
      crc.reset();
      crc.setPolynome(0x07);
      crc.add(startRByte1);
      crc.add(startRByte2);
      b_PRINT("\n Byte recived\n");
      b_PRINT_HEX(" ",startRByte1);
      b_PRINT_HEX(" ",startRByte2);
      for (int i = 0; i < 18; i++)
      {
        b_PRINT_HEX(" ",recieverByte[i]);
        recieverChar[i] = recieverByte[i];
        crc.add(recieverByte[i]);
      }
      ///////////////////////////////////
      

      if (crc.getCRC() == recieverByte[18]) { //compare CRC calculated to the reciever byte
        //If successfull save the byte data into variables
        
        for (int i = 0; i < 6; i = i + 2) { //save the designated byte into the right variable
            Pos_d[i / 2] = (recieverChar[i] << 8) + recieverByte[i + 1];
            rawcurTheta[i / 2] = (recieverChar[i + 6] << 8) + recieverByte[i + 7];
            rawcurDTheta[i / 2] = (recieverChar[i + 12] << 8) + recieverByte[i + 13];
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
