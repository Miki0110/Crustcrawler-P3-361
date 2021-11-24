
int xChange = 0;
int yChange = 0;
int zChange = 0;
byte  ReadedArray[4];



void KeyInput() {
  if (Serial1.read() == 254) {
    Serial1.readBytes(ReadedArray, 4);
  }
  int NewMovement[3];
  for (int i = 1; i <= 3; i++) {
    if (ReadedArray[i - 1] == 1) {
      NewMovement[i - 1] = -1;
    }
    else if (ReadedArray[i - 1] == 2) {
      NewMovement[i - 1] = 1;
    }
    else if (ReadedArray[i - 1] == 0) {
      NewMovement[i - 1] = 0;
    }
  };

  Move((int) NewMovement[0], (int) NewMovement[1], (int) NewMovement[2]);
  Gripper(ReadedArray[3]);
}

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
