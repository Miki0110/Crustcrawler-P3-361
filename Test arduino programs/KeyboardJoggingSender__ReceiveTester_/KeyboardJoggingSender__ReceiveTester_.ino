#include <SoftwareSerial.h>
SoftwareSerial soft_serial(12, 13); // RX/TX

byte currentAxis = 1; //1 = X, 2 = Y, 3 = Z
double desiredXPos = 0;
double desiredYPos = 0;
double desiredZPos = 0;
byte gripperState = 0; //0 = open, 1 = closed

double movementStep = 0.1; //How much to increment the value of an axis each time a command is received

//Bytes to receive in message
byte startByte = 0x7A;
byte dataArray[4] = {0, 0, 0, 0}; //Cycle axis, Axis +, Axis -, Gripper toggle

void setup() {
  //Start serial port for displaying recieved info
  Serial.begin(57600);
  while (!Serial); //Wait for serial port to be available

  //Start serial port for receiving commands
  soft_serial.begin(57600);
}

void loop() {
  // put your main code here, to run repeatedly:
  receivedInputs();
}

void receivedInputs() {
  //Read data from serial
  if (soft_serial.read() == startByte) {
    soft_serial.readBytes(dataArray, 4);
  }

  //Cycle axis
  if (dataArray[0] == 1) {
    if (currentAxis == 3) {
      currentAxis = 1;
    }
    else {
      currentAxis ++;
    }
    Serial.println("Axis cycled");
    Serial.print("New axis: ");
    Serial.println(currentAxis);
    dataArray[0] = 0;
  }

  //Axis +
  if (dataArray[1] == 1) {
    Serial.println("Axis +");
    if (currentAxis == 1) {
      desiredXPos += movementStep; //Increment X axis 1 movement step
      Serial.print("Axis 1 value: ");
      Serial.println(desiredXPos);
    } else if (currentAxis == 2) {
      desiredYPos += movementStep; //Increment Y axis 1 movement step
      Serial.print("Axis 2 value: ");
      Serial.println(desiredYPos);
    } else {
      desiredZPos += movementStep; //Increment Z axis 1 movement step
      Serial.print("Axis 3 value: ");
      Serial.println(desiredZPos);
    }
    dataArray[1] = 0;
  }
  
  //Axis -
  if (dataArray[2] == 1) {
    Serial.println("Axis -");
    if (currentAxis == 1) {
      desiredXPos -= movementStep; //Decrement X axis 1 movement step
      Serial.print("Axis 1 value: ");
      Serial.println(desiredXPos);
    } else if (currentAxis == 2) {
      desiredYPos -= movementStep; //Decrement Y axis 1 movement step
      Serial.print("Axis 2 value: ");
      Serial.println(desiredYPos);
    } else {
      desiredZPos -= movementStep; //Decrement Z axis 1 movement step
      Serial.print("Axis 3 value: ");
      Serial.println(desiredZPos);
    }
    dataArray[2] = 0;
  }

  //Gripper toggle
  if (dataArray[3] == 1) {
    if (gripperState == 0) {
      gripperState = 1;
    }
    else {
      gripperState = 0;
    }
    Serial.println("Gripper Toggled");
    dataArray[3] = 0;
  }
}
