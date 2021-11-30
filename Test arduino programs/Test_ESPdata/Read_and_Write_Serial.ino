boolean readInput(int timeOut) { //insert call function to the other arduino
  byte recieverByte[32];
  long currentMillis = millis();

  int rawThetaref[3], rawdThetaref[3], rawddThetaref[3], rawcurTheta[3], rawcurDTheta[3];

  while (true) {
    if (currentMillis + timeOut <= millis()) {
      return 0; //failed communication
      Serial.println("Failed");
    }
    if (Serial1.read() == startRByte) {
      Serial1.readBytes(recieverByte, 31);

      crc.reset();
      crc.setPolynome(0x07);
      crc.add(startRByte);
      for (int i = 0; i < 30; i++)
      {
        crc.add(recieverByte[i]);
      }
      Serial.print("CRC calced: ");
      Serial.println(crc.getCRC(), HEX);

      Serial.print("Last byte: ");
      Serial.println(recieverByte[30], HEX);
      
      if (crc.getCRC() == recieverByte[30]) { //should check the last bit
        for (int i = 0; i < 6; i = i + 2) {
          rawThetaref[i / 2] = (recieverByte[i] << 8) + recieverByte[i + 1];
          rawdThetaref[i / 2] = (recieverByte[i + 6] << 8) + recieverByte[i + 7];
          rawddThetaref[i / 2] = (recieverByte[i + 12] << 8) + recieverByte[i + 13];

          rawcurTheta[i / 2] = (recieverByte[i + 18] << 8) + recieverByte[i + 19];
          rawcurDTheta[i / 2] = (recieverByte[i + 24] << 8) + recieverByte[i + 25];
        }
        return 1;
      } else return 0;
    }
  }
}

void writeOutput(int outPut[3]){
byte returnValue[8]; //1 start byte + 3 PWM values + CRC

for(int i=0; i<3; i++){
  returnValue[0]=startByte;
    returnValue[i*2+1]=highByte(outPut[i]);
    returnValue[i*2+2]=lowByte(outPut[i]);
    }
  crc.reset();
  crc.setPolynome(0x07);
  for (int i = 0; i < 7; i++)
  {
    crc.add(returnValue[i]);
  }
    returnValue[7]=crc.getCRC();
    for(int i = 0; i < 8; i++){
      Serial.print(returnValue[i], HEX);
    soft_serial.write(returnValue[i]);
    }
    Serial.println("");
}
