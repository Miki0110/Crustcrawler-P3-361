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

//Add sEMG data to rolling averages
//sEMGch1.addValue(sEMGFetchedData[3]);
//sEMGch2.addValue(sEMGFetchedData[4]);

/*Following function interpreits a given command from sEMG channels
  Thresholds are set in main document as global variables for easier tuning
*/

//Time the channel signal has been held [unit sEMGInterpreterSampleTime in ms]
unsigned int sEMGch1HeldTime = 0;
unsigned int sEMGch2HeldTime = 0;

//Variables to remember the held time of last cycle
unsigned int sEMGch1LastHeldTime = 0;
unsigned int sEMGch2LastHeldTime = 0;

//Remember if gripper was just toggled
bool blockGripperToggle = false;

//sEMG interpreter function
void sEMGInterpreter() {
  //Command signal type recieved from sEMG channels
  //0 = no signal, 1 = impulse signal, 2 = long signal
  byte sEMGch1CommandSignal = 0;
  byte sEMGch2CommandSignal = 0;

  //If signal is held above the threshold the respective counter should be incremented
  if (sEMGch1.getAverage() >= sEMGch1Threshold) {
    sEMGch1HeldTime++;
  }
  if (sEMGch2.getAverage() >= sEMGch2Threshold) {
    sEMGch2HeldTime++;
  }

  //Determine what type of signal is sent on channel 1
  if (sEMGch1HeldTime <= sEMGch1LastHeldTime) {
    //Signal ended or not started
    if (sEMGch1HeldTime >= timeForHold) {
      bool blockGripperToggle = false;
    }
    else if (sEMGch1HeldTime > 0) {
      sEMGch1CommandSignal = 1; //Impulse signal recieved
    }
    sEMGch1HeldTime = 0;//Reset held time
  }
  else {
    //Signal still underway
    if (sEMGch1HeldTime >= timeForHold) {
      sEMGch1CommandSignal = 2; //Held signal recieved
    }
  }
  sEMGch1LastHeldTime = sEMGch1HeldTime;//Record held time for channel 1

  //Determine what type of signal is sent on channel 2
  if (sEMGch2HeldTime <= sEMGch2LastHeldTime) {
    //Signal ended or not started
    if (sEMGch2HeldTime >= timeForHold) {
      bool blockGripperToggle = false;
    }
    else if (sEMGch2HeldTime > 0) {
      sEMGch2CommandSignal = 1; //Impulse signal recieved
    }
    sEMGch2HeldTime = 0; //Reset held time
  }
  else {
    //Signal still underway
    if (sEMGch2HeldTime >= timeForHold) {
      sEMGch2CommandSignal = 2; //Held signal recieved
    }
  }
  sEMGch2LastHeldTime = sEMGch2HeldTime;//Record held time for channel 2

  //From both channel signals determine what command was sent
  if (sEMGch1CommandSignal == 1) {
    //Cycle Axis
    interpretedCommand = 1;
  }
  //else if (sEMGch2CommandSignal == 1) {
  //Emergency axis
  //  Serial.println("E-stop");
  //}
  else if (sEMGch1CommandSignal == 2 && sEMGch2CommandSignal == 0) {
    //Axis+
    interpretedCommand = 2;
  }
  else if (sEMGch1CommandSignal == 0 && sEMGch2CommandSignal == 2) {
    //Axis-
    interpretedCommand = 3;
  }
  else if (sEMGch1CommandSignal == 2 && sEMGch2CommandSignal == 2) {
    //Gripper Toggle
    interpretedCommand = 4;
    blockGripperToggle = true;
  }
}
