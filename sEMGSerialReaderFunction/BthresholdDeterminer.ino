/* Function to compare running average data to thresholds, and assigning threshold crossing with bool in FinalValue array
 * 
 * Only works when called in sEMGSerialReaderFunction.ino
 * 
 * Function requires FinalValue(array), Threshold(int), and sEMGfinal(int) to be defined before calling function
 * Data will be stored in array 
 */

//Function given same datatype as desired datatype
int thresHold(){

if(sEMGch1.getAverage() >= Threshold1){sEMGfinal1=1;} // This used to compare with the threshold
else{sEMGfinal1=0;}



if(sEMGch2.getAverage() >= Threshold2){sEMGfinal2=1;}// This used to compare with the threshold
else{sEMGfinal2=0;}

//Here the booleans are be added to the array - They have to be added on at a time

 FinalValues[1]={sEMGfinal1};
 FinalValues[2]={sEMGfinal2};

return FinalValues;
}
