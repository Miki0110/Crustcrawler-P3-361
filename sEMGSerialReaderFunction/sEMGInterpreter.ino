





void sEMGMessegeGenerater() {

  if (sEMGch1.getAverage() >= Threshold1) {
    sEMGch1HeldTime++;

    sEMGch1WaitingForMessage = 0;
  }

  else if (sEMGch1.getAverage() < Threshold1 && !sEMGch1WaitingForMessage) {
    sEMGNewMessage = 1;

    sEMGch1WaitingForMessage = 1;

  }

  if (sEMGch2.getAverage() >= Threshold2) {
    sEMGch2HeldTime++;

    sEMGch2WaitingForMessage = 0;
  }

  else if (sEMGch2.getAverage() < Threshold2 && !sEMGch2WaitingForMessage) {
    sEMGNewMessage = 1;

    sEMGch2WaitingForMessage = 1;


  }


}
// CHanges to function: Switchcase here
void sEMGInterpreter() {
  unsigned int signalArray[2] = {0, 0};

  if (sEMGNewMessage == 1) {
    if (sEMGch1HeldTime >= timeForHold) {
      signalArray[0] = 2;
    }
    else {
      signalArray[0] = 1;
    }

    sEMGch1HeldTime = 0;

    if (sEMGch2HeldTime >= timeForHold) {
      signalArray[1] = 2;
    }
    else {
      signalArray[1] = 1;
    }

    sEMGNewMessage = 0;
    sEMGch2HeldTime = 0;

  }

  if (signalArray[0] == 1 && signalArray[1] == 1) {
    //Cycle Axis
    Serial.println("Cycle Axis " ",");
  }
  else if (signalArray[0] == 2 && signalArray[1] == 0) {
    //Axis+
    Serial.print("Axis +");
  }
  else if (signalArray[0] == 0 && signalArray[1] == 2) {
    //Axis-
    Serial.print("Axis -");
  }
  else if (signalArray[0] == 2 && signalArray[1] == 2) {
    //Gripper Toggel
    Serial.println("Gripper Toggel");
  }
  //else if (signalArray[0] == 2 && signalArray[1] == 0) {
  //Emargncy Stop
  /*
    if (sEMGch1NewMessage==0&&sEMGch2NewMessage==0){
      signalArray[0]=0;
      signalArray[1]=0;
      //Serial.println(signalArray[1]);
      //Serial.println(signalArray[0]);
      }

  */


}// end void
