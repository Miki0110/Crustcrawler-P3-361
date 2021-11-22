





void sEMGMessegeGenerater() {

  if (sEMGch1.getAverage() >= Threshold1) {
    sEMGch1HeldTime++;

  }

  else if (sEMGch1.getAverage() < Threshold1) {
    sEMGch1HeldTime = 0;

    sEMGch1NewMessege = 1;

  }

  if (sEMGch2.getAverage() >= Threshold2) {
    sEMGch2HeldTime++;

  }

  else if (sEMGch2.getAverage() < Threshold2) {
    sEMGch2HeldTime = 0;

    sEMGch2NewMessege = 1;

  }


}
// CHanges to function: Switchcase here
void sEMGInterpreter() {
  unsigned char signalArray[2];

  if (sEMGch1NewMessege = 1) {
    if (sEMGch1HeldTime >= timeForHold) {
      signalArray[0] = 2;
    }
    else {
      signalArray[0] = 1;
    }

    sEMGch1NewMessege = 0;
    sEMGch1HeldTime = 0;

    if (sEMGch2HeldTime >= timeForHold) {
      signalArray[1] = 2;
    }
    else {
      signalArray[1] = 1;
    }

    sEMGch2NewMessege = 0;
    sEMGch2HeldTime = 0;

  }
  if (signalArray[0] == 1 && signalArray[1] == 1) {
    //Cycle Axis
    Serial.println("Cycle Axis" ",");
  }
  else if (signalArray[0] == 2 && signalArray[1] == 0) {
    //Axis+
    Serial.print("Axis");
  }
  else if (signalArray[0] == 0 && signalArray[1] == 2) {
    //Axis-
    Serial.print("Axis");
  }
  else if (signalArray[0] == 2 && signalArray[1] == 2) {
    //Gripper Toggel
    Serial.print("Gripper Toggel");
  }
  //else if (signalArray[0] == 2 && signalArray[1] == 0) {
  //Emargncy Stop
}// end void
