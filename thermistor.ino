include <SPI.h>
const int analogPin = 0;

const float B = 3950.0;
const float R0 = 10000.0;
const float Rd = 10000.0;
const float Tk = 273.15;

void setup()
{
    Serial.begin(9600);
}

void loop()
{
    float readValue = analogRead(analogPin);
    float Rt = Rd * readValue / (1023 - readValue);
    float Tbar = 1/B * log(Rt/R0) + 1/(Tk + 25);
    float T = 1/Tbar;
    float Tdeg = T - Tk;

    Serial.println(Tdeg);
    delay(200);
}