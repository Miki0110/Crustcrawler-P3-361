//Program for testing cartesian movement
//Using Keyboard inputs to jog robot

#include <SoftwareSerial.h>
SoftwareSerial soft_serial(12, 13); // RX/TX

//Bytes to send in message
byte startByte = 0x7A;
byte dataArray[4] = {0, 0, 0, 0}; //Cycle axis, Axis +, Axis -, Gripper toggle

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
      dataArray[0] = 1;
      break;
    case 'x':
      //Axis +
      dataArray[1] = 1;
      break;
    case 'c':
      //Axis -
      dataArray[2] = 1;
      break;
    case 'v':
      //Gripper toggle
      dataArray[3] = 1;
      break;
    default:
      //Invalid input
      break;
  }
}

void keyInputSender() {
  //Write first byte
  soft_serial.write(startByte);

  //Write data bytes
  for (int i = 0; i <= 3; i++) {
    soft_serial.write(dataArray[i]);
    //Serial.print("Writing...");
    //Serial.println(dataArray[i]);
    dataArray[i] = 0;
  }
}
