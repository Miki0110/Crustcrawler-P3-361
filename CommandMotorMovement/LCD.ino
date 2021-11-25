
void LCDPrint(){
  lcd.clear();
int TotalStringLenght=0;
String PrintString="";

  lcd.setCursor(0,0); //Print on first line
  lcd.print("Current Axis: ");
  lcd.print(currentAxis);
  Serial.println(currentAxis);

//  lcd.setCursor(0,1); //Print on second line
//lcd.print(TotalStringLenght/2 + PrintString);

  lcd.setCursor(0,1); //Print on second line
lcd.print("1 Loop Run Time: ");
lcd.print(millis()-lastCalcTime);
lcd.print(" ms");
  }
