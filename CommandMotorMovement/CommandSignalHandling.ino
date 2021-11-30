byte currentAxis = 1; //1 = X, 2 = Y, 3 = Z

//Bytes to receive in message
byte commandStartByte = 0x7A;


void actOnReceivedInputs(byte commandData) { //Set to 0 if debugging with serial

  //Read data from serial for debug
  //if (Serial1.read() == commandStartByte) {
  //  commandData = Serial1.read();
  //}

  switch (commandData) { //1 = Cycle axis, 2 = Axis +, 3 = Axis -, 4 = Gripper toggle
    case 1: //Cycle axis
      if (currentAxis == 3) {
        currentAxis = 1;
      }
      else {
        currentAxis ++;
      }
      //Serial.println("Axis cycled");
      //Serial.print("New axis: ");
      //Serial.println(currentAxis);
      break;
    case 2: //Axis +
      if (currentAxis == 1 && desiredXPos < HiLimitXPos) {
        desiredXPos += movementStep; //Increment X axis 1 movement step
        //Serial.print("Axis 1 value: ");
        //Serial.println(desiredXPos);
      } else if (currentAxis == 2 && desiredYPos < HiLimitYPos) {
        desiredYPos += movementStep; //Increment Y axis 1 movement step
        //Serial.print("Axis 2 value: ");
        //Serial.println(desiredYPos);
      } else if (desiredZPos < HiLimitZPos) {
        desiredZPos += movementStep; //Increment Z axis 1 movement step
        //Serial.print("Axis 3 value: ");
        //Serial.println(desiredZPos);
      }
      break;
    case 3: //Axis -
      if (currentAxis == 1 && desiredXPos > LoLimitXPos) {
        desiredXPos -= movementStep; //Decrement X axis 1 movement step
        //Serial.print("Axis 1 value: ");
        //Serial.println(desiredXPos);
      } else if (currentAxis == 2 && desiredYPos > LoLimitYPos) {
        desiredYPos -= movementStep; //Decrement Y axis 1 movement step
        //Serial.print("Axis 2 value: ");
        //Serial.println(desiredYPos);
      } else if (desiredZPos > LoLimitZPos){
        desiredZPos -= movementStep; //Decrement Z axis 1 movement step
        //Serial.print("Axis 3 value: ");
        //Serial.println(desiredZPos);
      }
      break;
    case 4: //Gripper toggle
      toggleGripper();
      //Serial.println("Gripper Toggled");
      break;
    default:
      break;
  }
}
