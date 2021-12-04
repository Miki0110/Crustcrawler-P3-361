void serialDoubleWrite(double d) {
  Wire.beginTransmission(4); // transmit to device #4
  String SerialData = "";

  SerialData = String(d, 4);

  Wire.write(SerialData.c_str());
  Wire.endTransmission();    // stop transmitting
}

void wireWrite(const char d) {
  Wire.beginTransmission(4); // transmit to device #4
  Wire.write(d);
  Wire.endTransmission();    // stop transmitting
}

bool callPWM(float Thetaref[3], float dThetaref[3], float ddThetaref[3]) {

  //////////////////////////////////////////////////////////////////////////////////////////////
  ////                            write all the data and send it                          //////
  byte startByte1 = 0x41;
  byte startByte2 = 0xDF;

  byte startRByte1 = 0x42;
  byte startRByte2 = 0xDF;

  byte positionMessage[33];
  int rawThetaref[3], rawdThetaref[3], rawddThetaref[3];
  int currTheta[3], currDTheta[3];
  //Read current positions
  for (int i = 0; i < 3; i++) {
    currTheta[i] = dxl.getPresentPosition(i + 1, UNIT_RAW);
    currDTheta[i] = dxl.getPresentVelocity(i + 1, UNIT_RAW);
    //convert wanted position to raw values
    rawThetaref[i] = Thetaref[i] / 0.088;
    rawdThetaref[i] = dThetaref[i] / 360 * 60 / 0.114;
    rawddThetaref[i] = ddThetaref[i] / 360 * 60 / 0.114;

    /* Serial.print("curr Theta ");
      Serial.print(i);
      Serial.print(": ");
      Serial.println(currTheta[i]);
      Serial.println(currDTheta[i]);*/
  }


  //convert to bytes and write
  positionMessage[0] = startByte1;
  positionMessage[1] = startByte2;
  for (int i = 0; i < 3; i++) {
    positionMessage[i * 2 + 2] = highByte(rawThetaref[i]);
    positionMessage[i * 2 + 3] = lowByte(rawThetaref[i]);
  }
  for (int i = 0; i < 3; i++) {
    positionMessage[i * 2 + 7] = highByte(rawdThetaref[i]);
    positionMessage[i * 2 + 8] = lowByte(rawdThetaref[i]);
  }
  for (int i = 0; i < 3; i++) {
    positionMessage[i * 2 + 13] = highByte(rawddThetaref[i]);
    positionMessage[i * 2 + 14] = lowByte(rawddThetaref[i]);
  }
  for (int i = 0; i < 3; i++) {
    positionMessage[i * 2 + 19] = highByte(currTheta[i]);
    positionMessage[i * 2 + 20] = lowByte(currTheta[i]);
  }
  for (int i = 0; i < 3; i++) {
    positionMessage[i * 2 + 25] = highByte(currDTheta[i]);
    positionMessage[i * 2 + 26] = lowByte(currDTheta[i]);
  }
  //add the CRC-check
  crc.reset();
  crc.setPolynome(0x07);
  for (int i = 0; i < 32; i++)
  {
    crc.add(positionMessage[i]);
  }
  positionMessage[32] = crc.getCRC();
  // Serial.println("sending: ");
  //Serial.println(" ");
  for (int i = 0; i < 33; i++) {
    //Serial.print(positionMessage[i], HEX);
    soft_serial.write(positionMessage[i]);
  }
  // Serial.println("");
  ////                                                                                    //////
  //////////////////////////////////////////////////////////////////////////////////////////////

  //Wait and read answer
  byte recieverByte[9];
  long currentMillis = millis();


  /* while (Serial.read() != startRByte){
     if (currentMillis + 200 <= millis()) {
       return 0;
     }
    }*/
  if (Serial2.read() == startRByte1) {
    if (Serial2.read() == startRByte2) {
      Serial2.readBytes(recieverByte, 7);

      crc.reset();
      crc.setPolynome(0x07);
      crc.add(startRByte1);
      crc.add(startRByte2);
      //Serial.println("Byte recived");
      for (int i = 0; i < 6; i++)
      {
        crc.add(recieverByte[i]);
        //Serial.print(recieverByte[i], HEX);
      }

      //Serial.println("");
      /*Serial.print("CRC calced: ");
        Serial.println(crc.getCRC(), HEX);

        Serial.print("Last byte: ");
        Serial.println(recieverByte[6], HEX);*/
      if (crc.getCRC() == recieverByte[6]) {
        for (int i = 0; i < 6; i = i + 2) {
          if (bitRead(recieverByte[i], 7) == 1) {
            PWMvalue[i / 2] = (recieverByte[i] << 8) + recieverByte[i + 1];

            bitWrite(PWMvalue[i / 2], 17, 0);
          } else {
            PWMvalue[i / 2] = (recieverByte[i] << 8) + recieverByte[i + 1];
          }
        }

        for (int i = 0; i < 3; i++) {
          setPWM(i + 1, PWMvalue[i]);
        }

        /*Serial.print("PWM: ");
          Serial.println(PWMvalue[0]);
          Serial.print("PWM: ");
          Serial.println(PWMvalue[1]);
          Serial.print("PWM: ");
          Serial.println(PWMvalue[2]);*/

        return 1;
      } else {
        counter++;;
        return 0;
      }
    }
  }
}