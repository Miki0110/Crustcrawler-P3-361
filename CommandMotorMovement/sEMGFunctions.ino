/*Call function to fetch data from sEMG
  Alters the content of the sEMGFetchedData array
  Returns true if data was fetched successfully, otherwise returns false
*/
bool fetchDataFromsEMG(int timeOutAfter) {
  byte fullMessageFromsEMG[24]; //Byte array for storing full wrapped message
  long currentMillis = millis();

  //Loop until message is recieved
  while (true) {
    if (currentMillis + timeOutAfter <= millis()) //Timeout if specified time in milliseconds is reached
    {
      //Failed to recieve message (Timed out)
      return false;
    }

    //Check serial bitstream for start bit 0x7E
    if (Serial1.read() == 0x7E) {
      //Record following bits into the message array
      Serial1.readBytes(fullMessageFromsEMG, 23);

      //Rearrange array to make room for start bit in message
      for (int i = 23; i > 0; i--) {
        fullMessageFromsEMG[i] = fullMessageFromsEMG[i - 1];
      }
      //Add start bit to message
      fullMessageFromsEMG[0] = 0x7E;

      //Print message for debug
      //for (int i = 0; i <= 23; i++) {
      //  Serial.print(fullMessageFromsEMG[i], HEX);
      //  Serial.print(" ");
      //}
      //Serial.println();

      //Calculate checksum (All bytes added together exclusive first 3)
      byte checksum = 0;
      for (int i = 3; i <= 23; i++) {
        checksum += fullMessageFromsEMG[i];
      }
      
      //Print checksum for debug
      //Serial.print("Checksum: ");
      //Serial.println(checksum, HEX);

      //If message is intact add sEMG data to rolling average
      if (checksum == 0xFF) {
        //Get data from array by combining msb and lsb into single variable
        int ch1 = fullMessageFromsEMG[20] + (fullMessageFromsEMG[19] << 8);
        int ch2 = fullMessageFromsEMG[22] + (fullMessageFromsEMG[21] << 8);

        //Print channel data for debug
        //Serial.print(ch1);
        //Serial.print(" ");
        //Serial.println(ch2);

        //Add sEMG data to rolling averages
        sEMGch1.addValue(ch1);
        sEMGch2.addValue(ch2);

        //Print channel average for debug
        //Serial.print(sEMGch1.getAverage());
        //Serial.print(" ");
        //Serial.println(sEMGch2.getAverage());
      }

      //Recieved message
      return true;
    }
  }
}

/*Following function interpreits a given command from sEMG channels
  Thresholds are set in main document as global variables for easier tuning
*/

//Time the channel signal has been held [unit sEMGInterpreterSampleTime in ms]
unsigned int sEMGch1HeldTime = 0;
unsigned int sEMGch2HeldTime = 0;

//Remember if gripper was just toggled
bool blockGripperToggle = false;

//sEMG interpreter function
void sEMGInterpreter() {

  //If signal is held above the threshold the respective counter should be incremented
  if (sEMGch1.getAverage() >= sEMGch1Threshold) { //Signal active
    sEMGch1HeldTime++;
    
    if (sEMGch1HeldTime >= timeForHold && sEMGch2HeldTime == 0) { //Held signal underway
      interpretedCommand = 2; //Axis+
    }
  }
  else { //Signal inactive
    if (sEMGch1HeldTime >= 1 && sEMGch1HeldTime <= timeForHold && sEMGch2HeldTime == 0) {
      interpretedCommand = 1; //Cycle Axis
    }
    sEMGch1HeldTime = 0;//Reset held time
  }
  
  if (sEMGch2.getAverage() >= sEMGch2Threshold) { //Signal active
    sEMGch2HeldTime++;
    
    if (sEMGch2HeldTime >= timeForHold && sEMGch1HeldTime == 0) { //Held signal underway
      interpretedCommand = 3; //Axis-
    }
  }
  else { //Signal inactive
    sEMGch2HeldTime = 0;//Reset held time
  }

  //Print Held time for debug
  //Serial.print(sEMGch1HeldTime);
  //Serial.print(" ");
  //Serial.println(sEMGch2HeldTime);

  if (sEMGch1HeldTime >= timeForHold && sEMGch2HeldTime >= timeForHold) {
    interpretedCommand = 4; //Gripper Toggle

    //Reset everything to avoid clapping
    sEMGch1HeldTime = 0;//Reset held time
    sEMGch2HeldTime = 0;//Reset held time
  }
}
