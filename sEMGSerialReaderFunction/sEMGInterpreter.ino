

//Time the channel has held time with unit: sEMGInterpreterSampleTime in ms
unsigned int sEMGch1HeldTime;
unsigned int sEMGch2HeldTime;

//Variables to keep track of the availability of new message
bool sEMGNewMessage1 = 0;
bool sEMGNewMessage2 = 0;

//Variables to keep track of message waiting status in each channel
bool sEMGch1WaitingForMessage = 1;
bool sEMGch2WaitingForMessage = 1;


void sEMGInterpreterHoldTracker() {
  if (sEMGch1.getAverage() >= Threshold1) {
    sEMGch1HeldTime++;

    sEMGch1WaitingForMessage = 0;
  }
  else if (sEMGch1.getAverage() < Threshold1 && !sEMGch1WaitingForMessage) {
    sEMGNewMessage1 = 1;

    sEMGch1WaitingForMessage = 1;
  }

  if (sEMGch2.getAverage() >= Threshold2) {
    sEMGch2HeldTime++;

    sEMGch2WaitingForMessage = 0;
  }
  else if (sEMGch2.getAverage() < Threshold2 && !sEMGch2WaitingForMessage) {
    sEMGNewMessage2 = 1;

    sEMGch2WaitingForMessage = 1;
  }
}

// Changes to function: Switchcase here
void sEMGInterpreter() {
  unsigned int signalArray[2] = {0, 0};

  if (sEMGch1WaitingForMessage && sEMGch2WaitingForMessage) {
    if (sEMGNewMessage1 == 1) {
      if (sEMGch1HeldTime >= timeForHold) {
        signalArray[0] = 2;
      }
      else {
        signalArray[0] = 1;
      }
      sEMGNewMessage1 = 0;
      sEMGch1HeldTime = 0;
    }

    if (sEMGNewMessage2 == 1) {
      if (sEMGch2HeldTime >= timeForHold) {
        signalArray[1] = 2;
      }
      else {
        signalArray[1] = 1;
      }
      sEMGNewMessage2 = 0;
      sEMGch2HeldTime = 0;
    }
  }

  if (signalArray[0] == 1 && signalArray[1] == 1) {
    //Cycle Axis
    Serial.println("Cycle Axis");
  }
  else if (signalArray[0] == 2 && signalArray[1] == 0) {
    //Axis+
    Serial.println("Axis +");
  }
  else if (signalArray[0] == 0 && signalArray[1] == 2) {
    //Axis-
    Serial.println("Axis -");
  }
  else if (signalArray[0] == 2 && signalArray[1] == 2) {
    //Gripper Toggel
    Serial.println("Gripper Toggle");
  }
}
