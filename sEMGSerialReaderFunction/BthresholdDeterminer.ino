/*
int Threshold1 = 135; //Orbiclaris oculi 75%
int Threshold2 = 120; //Frontalis 75%

boolean sEMGfinal1 = 0;
boolean sEMGfinal2 = 0;

*/


void thresHold(boolean getAverage){
  
//int getValue [2](){
if(sEMGch1.getAverage() >= Threshold1){sEMGfinal1=1;} // This used to cumper with the thresholdes
else{sEMGfinal1=0;}



if(sEMGch2.getAverage() >= Threshold2){sEMGfinal2=1;}// This used to cumper with the thresholdes
else{sEMGfinal2=0;}
//Here the booleans should be added to the array

int FinalValues[2]={sEMGfinal1, sEMGfinal2};

//FinalValues=sEMGfinal1,sEMGfinal2};

for(int i = 0; i < 2; i++)
{


Serial.println(FinalValues[i]);
}

return FinalValues;


}
