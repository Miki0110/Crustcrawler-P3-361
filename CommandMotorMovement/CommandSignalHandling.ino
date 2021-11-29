byte currentAxis = 1; //1 = X, 2 = Y, 3 = Z
byte gripperState = 0; //0 = open, 1 = closed

//Bytes to receive in message
byte startByte = 0x7A;


void receivedInputsFromSerial() {
  byte dataArray[4] = {0, 0, 0, 0}; //Cycle axis, Axis +, Axis -, Gripper toggle
  
  //Read data from serial
  if (Serial1.read() == startByte) {
    Serial1.readBytes(dataArray, 4);
  }

  //Cycle axis
  if (dataArray[0] == 1) {
    if (currentAxis == 3) {
      currentAxis = 1;
    }
    else {
      currentAxis ++;
    }
    dataArray[0] = 0;
    
    LCDPrint();
  }

  //Axis +
  if (dataArray[1] == 1) {
    if (currentAxis == 1) {
      desiredXPos += movementStep; //Increment X axis 1 movement step
    } else if (currentAxis == 2) {
      desiredYPos += movementStep; //Increment Y axis 1 movement step
    } else {
      desiredZPos += movementStep; //Increment Z axis 1 movement step
    }
    dataArray[1] = 0;
  }
  
  //Axis -
  if (dataArray[2] == 1) {
    if (currentAxis == 1) {
      desiredXPos -= movementStep; //Decrement X axis 1 movement step
    } else if (currentAxis == 2) {
      desiredYPos -= movementStep; //Decrement Y axis 1 movement step
    } else {
      desiredZPos -= movementStep; //Decrement Z axis 1 movement step
    }
    dataArray[2] = 0;
  }

  //Gripper toggle
  if (dataArray[3] == 1) {
    toggleGripper();
    dataArray[3] = 0;
  }
}
