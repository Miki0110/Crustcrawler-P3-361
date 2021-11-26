//This is for an LCD using an I2C adapter
void LCDPrint(){
  lcd.clear();
int TotalStringLenght=0;
String PrintString="";

  lcd.setCursor(0,0); //Print on first line
  lcd.print("Current Axis: ");
  lcd.setCursor(14,0);
  lcd.print(currentAxis);
  Serial.println(currentAxis);

//  lcd.setCursor(0,1); //Print on second line
//lcd.print(TotalStringLenght/2 + PrintString);

  lcd.setCursor(0,1); //Print on second line
lcd.print("Time: ");
  lcd.setCursor(6,1);
lcd.print(millis()-lastCalcTime);
  lcd.setCursor(13,1);
lcd.print(" ms");
  }
