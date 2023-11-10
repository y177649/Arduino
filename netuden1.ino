#include <SPI.h>
int value;

void setup() {
  SPI.setBitOrder(MSBFIRST);
  SPI.setClockDivider(SPI_CLOCK_DIV4);
  SPI.setDataMode(SPI_MODE0);
  SPI.begin();
  Serial.begin(9600);
}

void loop() { 
  delay(500);
  digitalWrite(SS, LOW);
  value = SPI.transfer(0x00) << 8;
  value |= SPI.transfer(0x00);
  digitalWrite(SS, HIGH);

  if ((value & 0x0004) != 0){
    Serial.println("Open Error");
    
  } else {
    //Serial.print((value >> 3), BIN);
    //Serial.print(", ");
    Serial.print((value >> 3) * 0.25);
    Serial.println(" degC");
  } 
  
}