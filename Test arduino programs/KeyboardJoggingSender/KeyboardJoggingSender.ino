//Program for testing cartesian movement
//Using Keyboard inputs to jog robot

#include <SoftwareSerial.h>
SoftwareSerial soft_serial(12, 13); // RX/TX

//Bytes to send in message
byte startByte = 0x7A;
byte dataArray[4] = {0, 0, 0, 0}; //Cycle axis, Axis +, Axis -, Gripper toggle

unsigned int lastMillis;
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
  if (millis() >= lastMillis + sampleTime) {
    keyInputReader();
    keyInputSender();
  }
}

void keyInputReader() {
  char keyInput = Serial.read();
  Serial.println(keyInput);

  for (int i = 0; i <= 3; i++) {
    dataArray[i] = 0;
  }

  switch (keyInput) {
    case 'z':
      //Cycle axis
      dataArray[0] = 255;
      break;
    case 'x':
      //Axis +
      dataArray[1] = 255;
      break;
    case 'c':
      //Axis -
      dataArray[2] = 255;
      break;
    case 'v':
      //Gripper toggle
      dataArray[3] = 255;
      break;
    default:
      //Invalid input
  }
}

void keyInputSender() {
  //Write first byte
  soft_serial.write(startByte);

  //Write data bytes
  for (int i = 0; i <= 3; i++) {
    soft_serial.write(dataArray[i]);
  }
}
