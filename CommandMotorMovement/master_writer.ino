void serialDoubleWrite(double d) {
Wire.beginTransmission(4); // transmit to device #4
String SerialData="";

SerialData = String(d,4);

Wire.write(SerialData.c_str());
Wire.endTransmission();    // stop transmitting
}

void wireWrite(const char d){
  Wire.beginTransmission(4); // transmit to device #4
  Wire.write(d);
  Wire.endTransmission();    // stop transmitting
  }

bool callPWM(float Thetaref[3], float dThetaref[3], float ddThetaref[3]) {

//////////////////////////////////////////////////////////////////////////////////////////////
////                            write all the data and send it                          //////
  byte startByte = 0x5A; //send byte
  byte startRByte = 0x41; //reciver byte
  byte positionMessage[32];
  int rawThetaref[3], rawdThetaref[3], rawddThetaref[3];
  int currTheta[3], currDTheta[3];
  //Read current positions
  for (int i = 0; i < 3; i++) {
    currTheta[i] =dxl.getPresentPosition(i+1, UNIT_RAW);
    currDTheta[i] =dxl.getPresentVelocity(i+1, UNIT_RAW);
    //convert wanted position to raw values
    rawThetaref[i]=Thetaref[i]/ 0.088;
    rawdThetaref[i]=dThetaref[i]/360*60/ 0.114;
    rawddThetaref[i]=ddThetaref[i]/360*60/ 0.114;

   /* Serial.print("curr Theta ");
    Serial.print(i);
    Serial.print(": ");
    Serial.println(currTheta[i]);
    Serial.println(currDTheta[i]);*/
  }
 

  //convert to bytes and write
  positionMessage[0]=startByte;
  for (int i = 0; i < 3; i++) {
    positionMessage[i*2+1]=highByte(rawThetaref[i]);
    positionMessage[i*2+2]=lowByte(rawThetaref[i]);
  }
  for (int i = 0; i < 3; i++) {
    positionMessage[i*2+6]=highByte(rawdThetaref[i]);
    positionMessage[i*2+7]=lowByte(rawdThetaref[i]);
  }
  for (int i = 0; i < 3; i++) {
    positionMessage[i*2+12]=highByte(rawddThetaref[i]);
    positionMessage[i*2+13]=lowByte(rawddThetaref[i]);
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
 // Serial.println("sending: ");
//Serial.println(" ");
    for (int i = 0; i < 32; i++){
    //Serial.print(positionMessage[i], HEX);
    soft_serial.write(positionMessage[i]);
    }
 // Serial.println("");
////                                                                                    //////
//////////////////////////////////////////////////////////////////////////////////////////////

  //Wait and read answer
  byte recieverByte[8];
  long currentMillis = millis();

  int16_t PWMvalue[3];

 /* while (Serial.read() != startRByte){
    if (currentMillis + 200 <= millis()) {
      return 0;
    }
  }*/
  if(Serial2.read() == startRByte){
  Serial2.readBytes(recieverByte, 7);
  
      crc.reset();
      crc.setPolynome(0x07);
      crc.add(startRByte);
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
  
    bitWrite(PWMvalue[i/2], 17, 0);
  }else{
    PWMvalue[i / 2] = (recieverByte[i] << 8) + recieverByte[i + 1];}
  }

for(int i =0; i < 3; i++){
  setPWM(i+1, PWMvalue[i]);
  }
      /*Serial.print("PWM: ");
      Serial.println(PWMvalue[0]);
      Serial.print("PWM: ");
      Serial.println(PWMvalue[1]);
      Serial.print("PWM: ");
      Serial.println(PWMvalue[2]);*/
  
  return 1;
 }else{
  counter++;;
  return 0;
  }
}
}
