

/*---------------------Timer-----------------------------
RunningAverage totalAverage(2); //add data here
int sampleTime = 1*1000; //add seconds here
int totalAverageCounter = 1;
unsigned long currentMillis;
/*---------------------Timer-----------------------------*/

void sampleTimeVal(){

 if(millis() >= currentMillis+sampleTime){   

for(int k = 1; k <= 2; k++){
FinalValues[k-1] = Fvloop[change];}
change++;

if (change > ((sizeof(Fvloop) / sizeof(Fvloop[0])))){change = 0;
  } //simlater
  
 for(int i = 1; i <= 2; i++){
  FinalValues[i-1];

    totalAverage.addValue(FinalValues[i-1]);
    totalAverageCounter ++;
    for (int j = i; j <= 8; j=j+2){
    OldFinalValues[j-1] = OldFinalValues[(j+2)-1];  // store what was read last time
}
    OldFinalValues[8+i-1] = FinalValues[i-1];  // store what was read last time
    //Serial.println(OldFinalValues[8+i-1]);
    for (int number = 0; number <=9; number++){}//Serial.print(OldFinalValues[number]);}
    /*if (FinalValues[i-1] == 1){
    Serial.print("Hold");
    Serial.print(i);
    Serial.print(", ");
    }
    else {
      Serial.print("0");
          Serial.print(", ");
    }*/

    //Serial.println();
    currentMillis = millis();
}
  //sEMG1 = 0,2,4,6,8
  //sEMG2 = 1,3,5,7,9
  
if (OldFinalValues [0]==0 && OldFinalValues [2]==0 && OldFinalValues [4]==0 && OldFinalValues [6]==1 && OldFinalValues[8]==0 ){ impuls1=1;  Serial.println("impuls1");}

   if (OldFinalValues [1]==0 && OldFinalValues [3]==0 && OldFinalValues [5]==0 && OldFinalValues [7]==1 && OldFinalValues[9]==0 ){ impuls2=1; Serial.println("impuls2");}
  
if (OldFinalValues [0]==0 && OldFinalValues [2]==0 && OldFinalValues [4]==1 && OldFinalValues [6]==1 && OldFinalValues [8]==1 ){impuls1=2;
   Serial.println("Hold1");}
   
    if (OldFinalValues [1]==0 && OldFinalValues [3]==0 && OldFinalValues [5]==1 && OldFinalValues [7]==1 && OldFinalValues [9]==1 ){impuls2=2;
   Serial.println("Hold2");}

//impuls1 = 1; Hold1 = 2 ; None=0; impuls2 = 1; Hold2 = 2
   if (impuls1 == 2 && impuls2 == 2){var = 1; } //NØDESTOP
    if (impuls1 == 2 && impuls2 == 2){var = 2; } //right
     if (impuls1 == 2 && impuls2 == 2){var = 3; } //left
      if (impuls1 == 2 && impuls2 == 2){var = 4; } //up
       if (impuls1 == 2 && impuls2 == 2){var = 5; } //down

    
/*---------------------Funcion-----------------------------*/

} //Time if 

 } //end loop
