
int xChange = 0;
int yChange = 0;
int zChange = 0;



void KeyInput() {

  while (1 < Wire.available()) { // loop through all but the last
    char c = Wire.read(); // receive byte as a character
    Serial.print(c);         // print the character
  };
  int x = Wire.read();    // receive byte as an integer
  Serial.println(x);         // print the integer
};

/*
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
      ActivateGripper();
      break;
    case '0':
      xChange = 0; yChange = 0; zChange = 0;
      break;
  }

  }

  Move(xChange, yChange, zChange);
}*/
