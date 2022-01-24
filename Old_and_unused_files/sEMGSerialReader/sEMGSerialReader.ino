/*
 * This code is a simple serial reader for collecting data from the sEMG 
 * The recorded message is saved in the message array
 */

int messageBytes = 24;//Message length
byte message[24];     //Buffer array for read bits

//Debug message with Serial monitor
bool printFullDebugMessage = false;
bool printDataDebugMessage = true;

int outputInterval = 1000; //Time between sending message to Serial in milliseconds
long lastOutputTime = 0;   //

//Debug print to serial monitor
void printFullMessage() //Print the full recorded message to the Serial monitor
{ 
  for (int i = 0; i < messageBytes-1; i++)  //Print all bits in message
  {  
    Serial.print(message[i], HEX);
    Serial.print(' ');
  }
    Serial.println(); //Prepare line for next message
}

void printDataMessage() //Print the data values to the serial monitor
{
  //Combining msb and lsb into single variable by bitshifting
  int accZData = message[14] + (message[13] << 8);
  int accYData = message[16] + (message[15] << 8);
  int accXData = message[18] + (message[17] << 8);
  int emg1Data = message[20] + (message[19] << 8);
  int emg2Data = message[22] + (message[21] << 8);

  Serial.print("Z Acceleration: ");
  Serial.print(accZData, DEC);
  Serial.println();
  Serial.print("Y Acceleration: ");
  Serial.print(accYData, DEC);
  Serial.println();
  Serial.print("X Acceleration: ");
  Serial.print(accXData, DEC);
  Serial.println();
  Serial.print("EMG channel 1: ");
  Serial.print(emg1Data, DEC);
  Serial.println();
  Serial.print("EMG channel 2: ");
  Serial.print(emg2Data, DEC);
  Serial.println();
}

void setup() 
{
  Serial.begin(115200); //Start serial port for debug
  while (!Serial) {}    //Wait for serial port to be active
  Serial1.begin(115200);//Start serial port for recieving message from XBee
  while (!Serial1) {}   //Wait for serial port to be active
}

void loop() 
{
  if (Serial1.read() == 0x7E) //Check serial bitstream for start bit 0x7E
  { 
    Serial1.readBytes(message, messageBytes-1); //Record following bits into the message array

    for (int i = 23; i > 0; i--)  //Rearrange array to make room for start bit in message
    {
      message[i] = message[i-1];
    }
    message[0] = 0x7E;  //Add start bit to message
  }

  //Print debug messages if enabled
  if (printFullDebugMessage && millis() > lastOutputTime + outputInterval) 
  {
    printFullMessage();
    lastOutputTime = millis();
  }
  if (printDataDebugMessage && millis() > lastOutputTime + outputInterval) 
  {
    printDataMessage();
    lastOutputTime = millis();
  }
}
