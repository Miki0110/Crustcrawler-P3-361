
#include <SoftwareSerial.h>


SoftwareSerial soft_serial(12, 13);


int xChange = 0;
int yChange = 0;
int zChange = 0;
int gripperChange = 0;

void setup() {


  soft_serial.begin(9600);
  Serial.begin(9600);
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
        xChange = 1;
        break;
      case 'd':
        xChange = 2;
        break;
      case 'w':
        yChange = 2;
        break;
      case 's':
        yChange = 1;
        break;
      case 'r':
        zChange = 2;
        break;
      case 'f':
        zChange = 1;
        break;
      case '1':
        gripperChange = 1;
        break;
      case '2':
        gripperChange = 0;
        break;
      default :
        xChange = 0; yChange = 0; zChange = 0;
        break;
    }

  }
  //0=0, 1=negativ,2=positiv
  soft_serial.write(254);
  soft_serial.write(xChange);
  soft_serial.write(yChange);
  soft_serial.write(zChange);
  soft_serial.write(gripperChange);

}
