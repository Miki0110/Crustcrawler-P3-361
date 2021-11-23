/* Function to fetch data recieved with XBee from the sEMG module

   XBee must be connected to Serial1 port on Arduino Mega 2560

   Function requires sEMGFetchedData integer array of 5 elements to be defined before calling function
   Data will be stored in array
*/

int Threshold1 = 535; //Orbiclaris oculi 75% = 135
int Threshold2 = 520; //Frontalis 75% = 120

bool sEMGfinal1 = 0; //Global for when threshold reached for channel one
bool sEMGfinal2 = 0; //Global for when threshold reached for channel two

int sEMGFetchedData[5]; //Array for storing newest data fetched from sEMG

int sEMGInterpreterSampleTime = 10; //Time between interpreter mesurements in miliseconds
unsigned long sEMGInterpreterTime;  //Counter to keep track of milliseconds between the interpreiter sample collection

unsigned int timeForHold = 100; //Time for impulse to be held, unit is sEMGInterpreterSampleTime in ms

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

  //Ensure clean slate for all rolling averages
  sEMGch1.clear();
  sEMGch2.clear();

  //Prepare interpreiter timer for use
  sEMGInterpreterTime = millis();
}

//Debug
//unsigned long debugMillis;

void loop()
{
  //Fetch data
  fetchDataFromsEMG(100);

  //Add data to rolling averages
  sEMGch1.addValue(sEMGFetchedData[3]);
  sEMGch2.addValue(sEMGFetchedData[4]);

  if (millis() >= sEMGInterpreterTime + sEMGInterpreterSampleTime) {
    sEMGInterpreterHoldTracker();
    sEMGInterpreterTime = millis();
  }
  sEMGInterpreter();
  
  //Debug
  /*
    if (millis() >= debugMillis + 500) {
    Serial.print(sEMGch1.getAverage(), 0);
    Serial.print(", ");
    Serial.println(sEMGch2.getAverage(), 0);
    debugMillis = millis();
    }
  */
}
