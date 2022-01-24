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
  if (sEMGch1.getAverage() >= Threshold1) {
    sEMGch1HeldTime++;
  }
  if (sEMGch2.getAverage() >= Threshold2) {
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
    sEMGch2HeldTime = 0;//Reset held time
  }
  else {
    //Signal still underway
    if (sEMGch2HeldTime >= timeForHold) {
      sEMGch2CommandSignal = 2; //Held signal recieved
    }
  }
  sEMGch2LastHeldTime = sEMGch2HeldTime;//Record held time for channel 2


  if (sEMGch1CommandSignal == 1) {
    //Cycle Axis
    Serial.println("Cycle Axis");
  }
  //else if (sEMGch2CommandSignal == 1) {
    //Emergency axis
  //  Serial.println("E-stop");
  //}
  else if (sEMGch1CommandSignal == 2 && sEMGch2CommandSignal == 0) {
    //Axis+
    Serial.println("Axis +");
  }
  else if (sEMGch1CommandSignal == 0 && sEMGch2CommandSignal == 2) {
    //Axis-
    Serial.println("Axis -");
  }
  else if (sEMGch1CommandSignal == 2 && sEMGch2CommandSignal == 2) {
    //Gripper Toggle
    Serial.println("Gripper Toggle");
    blockGripperToggle = true;
  }
}
