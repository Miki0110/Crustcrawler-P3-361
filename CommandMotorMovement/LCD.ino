//This is for an LCD using an I2C adapter
void LCDPrint(){
  //lcd.clear();
  lcd.setCursor(0,0); //Print on first line
  lcd.print("Current Axis: ");
  //lcd.setCursor(14,0);
  lcd.print(currentAxis);
  Serial.println(currentAxis);
  lcd.setCursor(0,1); //Print on second line
lcd.print("X:");
lcd.print((int) desiredXPos);
lcd.print("Y:");
lcd.print((int) desiredYPos);
lcd.print("Z:");
lcd.print((int) desiredZPos);
lcd.print(" ms  ");
  }
