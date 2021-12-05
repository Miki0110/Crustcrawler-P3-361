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

bool callPWM(int CartPos_d[3]) { //Desired coordinate should be in mm
  //////////////////////////////////////////////////////////////////////////////////////////////
  ////                            write all the data and send it                          //////
  byte startByte1 = 0x41;
  byte startByte2 = 0xDF;

  byte startRByte1 = 0x42;
  byte startRByte2 = 0xDF;

  byte positionMessage[21]; // 2 start bytes + 12 bytes for current pos & vel + 6 bytes for desired pos + 1 CRC byte
 
  int currTheta[3], currDTheta[3];
  //Read current positions
  for (int i = 0; i < 3; i++) {
    currTheta[i] = dxl.getPresentPosition(DXL_ID[i+1], UNIT_RAW);
    currDTheta[i] = dxl.getPresentVelocity(DXL_ID[i+1], UNIT_RAW);
  }


  //convert to bytes and write
  ////// Package structure /////////
  /*| Byte 1 | Byte 2  | Byte 3  | Byte 4  | Byte 5   | Byte 6   | Byte 7   | Byte 8   |  Byte 9  | Byte 10  |
    |Start1  | Start2  | Pos1H   | Pos1L   | Pos2H    | Pos2L    | Pos3H    | Pos3L    | Theta1H  | Theta1L  |
    
    |Byte 11 | Byte 12 | Byte 13 | Byte 14 | Byte 15  | Byte 16  | Byte 17  | Byte 18  | Byte 19  | Byte 20  | Byte 21|
    |Theta2H | Theta2L | Theta3H | Theta3L | dTheta1H | dTheta1L | dTheta2H | dTheta2L | dTheta3H | dTheta3L | CRC    | */
    
  positionMessage[0] = startByte1;
  positionMessage[1] = startByte2;
  for (int i = 0; i < 3; i++) {
    positionMessage[i * 2 + 2] = highByte(CartPos_d[i]);
    positionMessage[i * 2 + 3] = lowByte(CartPos_d[i]);

    positionMessage[i * 2 + 8] = highByte(currTheta[i]);
    positionMessage[i * 2 + 9] = lowByte(currTheta[i]);

    positionMessage[i * 2 + 14] = highByte(currDTheta[i]);
    positionMessage[i * 2 + 15] = lowByte(currDTheta[i]);
  }

  //add the CRC-check
  crc.reset();
  crc.setPolynome(0x07);
  for (int i = 0; i < 20; i++)
  {
    crc.add(positionMessage[i]);
  }
  positionMessage[20] = crc.getCRC();
  for (int i = 0; i < 21; i++) {
    soft_serial.write(positionMessage[i]); //Write everything to the ESP
  }
  
  ////                                                                                    //////
  //////////////////////////////////////////////////////////////////////////////////////////////

  //Wait and read answer
  byte recieverByte[9];
  long currentMillis = millis();
  
  int16_t PWMvalue[3];
  
  if (Serial2.read() == startRByte1) { //Check for first start byte
    if (Serial2.read() == startRByte2) { //Check for second start byte
      Serial2.readBytes(recieverByte, 7); //Write the next seven bytes into reciverByte
      
      /////////CRC-check////////////////
      crc.reset();  
      crc.setPolynome(0x07);
      crc.add(startRByte1);
      crc.add(startRByte2);
      for (int i = 0; i < 6; i++)
      {
        crc.add(recieverByte[i]);
      }
     /////////////////////////////////// 
      
      
      if (crc.getCRC() == recieverByte[6]) { //compare CRC calculated to the reciever byte
        //If successfull save the byte data into PWM value
        for (int i = 0; i < 6; i = i + 2) {
          
          if (bitRead(recieverByte[i], 7) == 1) { //Incase there's a signed bit indicating a negative value
            PWMvalue[i / 2] = (recieverByte[i] << 8) + recieverByte[i + 1];
            bitWrite(PWMvalue[i / 2], 17, 0); //Rewritting the byte so it understands it's a signed number and not 32768
          } else {
            PWMvalue[i / 2] = (recieverByte[i] << 8) + recieverByte[i + 1]; //If the bit is not signed just write it down
          }
        }

        for (int i = 0; i < 3; i++) { //Tell the motors to act accordingly
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
        counter++; //Debug counter (Delete later)
        return 0;
      }
    }
  }
}
