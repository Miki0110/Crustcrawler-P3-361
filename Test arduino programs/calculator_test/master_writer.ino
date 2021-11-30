void serialDoubleWrite(double d) {
Wire.beginTransmission(4); // transmit to device #4
String SerialData="";

SerialData = String(d,4);

Wire.write(SerialData.c_str());
Wire.endTransmission();    // stop transmitting
}

void wireWrite(const char d){
  Wire.beginTransmission(4); // transmit to device #4
  Wire.write(d);
  Wire.endTransmission();    // stop transmitting
  }
