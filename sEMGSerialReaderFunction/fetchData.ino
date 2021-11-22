




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
