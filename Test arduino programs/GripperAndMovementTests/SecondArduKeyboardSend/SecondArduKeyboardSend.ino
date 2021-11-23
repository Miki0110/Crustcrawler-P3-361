
#include <Wire.h>

int xChange = 0;
int yChange = 0;
int zChange = 0;
int gripperChange = 0;

void setup() {



  Wire.begin();
  Serial.begin(57600);
  while (!Serial); //Wait for serial port to be available

}

void loop() {

  if (Serial.available() > 0) {
    // read the incoming byte:
    char incomingchar = Serial.read();

    // say what you got:
    Serial.print("I received: ");
    Serial.println(incomingchar);
    switch (incomingchar) {
      case 'a':
        xChange = -1;
        break;
      case 'd':
        xChange = 1;
        break;
      case 'w':
        yChange = 1;
        break;
      case 's':
        yChange = -1;
        break;
      case 'r':
        zChange = 1;
        break;
      case 'f':
        zChange = -1;
        break;
      case '1':
        gripperChange = !gripperChange;
        break;
      case '0':
        xChange = 0; yChange = 0; zChange = 0;
        break;
    }

  }
  Wire.beginTransmission(8); // transmit to device #8
  Wire.write('x');
  Wire.write(xChange);              // sends one byte
  Wire.write('y');
  Wire.write(yChange);              // sends one byte
  Wire.write('z');
  Wire.write(zChange);              // sends one byte
  Wire.write('g');
  Wire.write(gripperChange);              // sends one byte
  Wire.endTransmission();    // stop transmitting
}
