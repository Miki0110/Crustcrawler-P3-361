/* Function to fetch data recieved with XBee from the sEMG module
 * 
 * XBee must be connected to Serial1 port on Arduino Mega 2560
 * 
 * Function requires sEMGFetchedData integer array of 5 elements to be defined before calling function
 * Data will be stored in array 
 */
int sEMGFetchedData[5]; //Array for storing newest data fetched from sEMG

int triggerPointch1 = 800;  //Trigger point for sEMG channel 1
int triggerPointch2 = 800;  //Trigger point for sEMG channel 2

//Running average library by Rob Tillaart
#include <RunningAverage.h>

//Define rolling averages and sample amounts (keep sample amounts low (<50))
RunningAverage sEMGch1(25);
RunningAverage sEMGch2(25);

void setup() 
{
  //Should be added to setup of arduino program calling this function
  Serial1.begin(115200);//Start serial port for recieving message from XBee
  while (!Serial1) {}   //Wait for serial port to be active
   Serial.begin(115200);//Start serial port for recieving message from XBee
  while (!Serial) {}   //Wait for serial port to be active

  //Testing
  pinMode(52, OUTPUT);
  pinMode(53, OUTPUT);

  //Ensure clean slate for all rolling averages
  sEMGch1.clear();
  sEMGch2.clear();
}

void loop() 
{
  //Fetch data
  fetchDataFromsEMG(100);

  //Add data to rolling averages
  sEMGch1.addValue(sEMGFetchedData[3]);
  sEMGch2.addValue(sEMGFetchedData[4]);

  //Print to serial plotter/monitor
  //Serial.print(sEMGFetchedData[3]);
  //Serial.print(", ");
  //Serial.print(sEMGFetchedData[4]);
  //Serial.print(", ");
  Serial.print(sEMGch1.getAverage(), 0);
  Serial.print(", ");
  Serial.println(sEMGch2.getAverage(), 0);
  

  //Testing set pins to high when threshold is met
  if(     sEMGch1.getAverage() >= triggerPointch1) digitalWrite(52, HIGH);
  else if(sEMGch1.getAverage() <  triggerPointch1) digitalWrite(52,  LOW);
  if(     sEMGch2.getAverage() >= triggerPointch2) digitalWrite(52, HIGH);
  else if(sEMGch2.getAverage() <  triggerPointch2) digitalWrite(52,  LOW);
}

//Call function to fetch data from sEMG
//Alters the content of the sEMGFetchedData array
//Returns true if data was fetched successfully, otherwise returns false
bool fetchDataFromsEMG(int timeOutAfter)
{
  byte fullMessageFromsEMG[24]; //Byte array for storing full wrapped message
  long currentMillis = millis();
  
  //Loop until message is recieved
  while(true) {
    if (currentMillis + timeOutAfter <= millis()) //Timeout if specified time in milliseconds is reached
    {
      return false; //Failed to recieve message (Timed out)
    }
    
    if (Serial1.read() == 0x7E) //Check serial bitstream for start bit 0x7E
    { 
      Serial1.readBytes(fullMessageFromsEMG, 23); //Record following bits into the message array
  
      for (int i = 23; i > 0; i--)  //Rearrange array to make room for start bit in message
      {
        fullMessageFromsEMG[i] = fullMessageFromsEMG[i-1];
      }
      fullMessageFromsEMG[0] = 0x7E;//Add start bit to message

      for (int i = 22; i >= 14; i -= 2) //Write data to sEMGFetchedData
      { 
        //Combine msb and lsb into single variable
        sEMGFetchedData[(i - 14)/2] = fullMessageFromsEMG[i] + (fullMessageFromsEMG[i - 1] << 8);
      }
      return true;//Recieved message
    }
  }
}
