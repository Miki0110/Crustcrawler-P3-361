//Program for testing cartesian movement
//Using Keyboard inputs to jog robot

#include <SoftwareSerial.h>
SoftwareSerial soft_serial(12, 13); // RX/TX

//Bytes to send in message
byte startByte = 0x7A;
byte commandData = 0; //1 = Cycle axis, 2 = Axis +, 3 = Axis -, 4 = Gripper toggle

unsigned long lastMillis;
int sampleTime = 10; //Time between keyboard samples and data packet send (in ms)

void setup() {
  //Start serial port for keyboard inputs
  Serial.begin(57600);
  while (!Serial); //Wait for serial port to be available

  //Start serial port for sending commands
  soft_serial.begin(57600);

  lastMillis = millis();
}

void loop() {  
  keyInputReader();
  if (millis() >= lastMillis + sampleTime) {
    keyInputSender();

    lastMillis = millis();
  }
}

void keyInputReader() {
  char keyInput = Serial.read();

  switch (keyInput) {
    case 'z':
      //Cycle axis
      commandData = 1;
      break;
    case 'x':
      //Axis +
      commandData = 2;
      break;
    case 'c':
      //Axis -
      commandData = 3;
      break;
    case 'v':
      //Gripper toggle
      commandData = 4;
      break;
    default:
      //Invalid input
      break;
  }
}

void keyInputSender() {
  //Write first byte
  soft_serial.write(startByte);

  //Write data byte
  soft_serial.write(commandData);
  commandData = 0;
}
