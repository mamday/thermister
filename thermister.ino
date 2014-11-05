#include "SPI.h"
#include "C:\Program Files (x86)\Arduino\libraries\WS2801\WS2801.h"




//Change these variables as much as you want

#define MinTemp 70       //The minimum temperature shown on the thermometer, when no lights are on
#define MaxTemp 90       //The maximum temperature shown, when all 20 lights are on









//This is the code for the thermometer




//Include the libraries to make the leds work


//Values for this particular thermistor setup
#define Resistor 10000
#define Volts 5
#define LedCount 20

//Telling the arduino which pins the lights are plugged in to
WS2801 LedBoard = WS2801(LedCount, 2, 3);

//Thermistor numbers, these need to be calibrated to get accurate temperature readings
//These are not calibrated at all  :|
double a = 0.001129148;
double b = 0.000234125;
double c = 0.0000000876741;
int i;                       //For loops
int LedsLit = 0;             //Remembers how many leds are currently lit up on the thermometer

//This happens once on startup
void setup() {
  
  //Initialize the light string
  LedBoard.begin();
  LedBoard.show();
}

//Do this over and over at 10 Hz
void loop() {
  //analogRead returns 0-1023
  //0 = 0V, 1023 = 5V
  //VoltageDrop is across the thermistor
  double VoltageDrop = Volts - (analogRead(A3) * 5.0 / 1024.0);
  
  //Convert VoltageDrop into resistance of the Thermistor
  double ThermistorResistance = Resistor * VoltageDrop / (Volts - VoltageDrop);
  
  //Steinhart-Hart equation
  //1 / T = a + b*ln(R) + c*ln(R)^3
  //Where T = temerature (Kelvin), R = resistance (Ohms)
  //a,b,c are constants for your particular thermistor
  double Kelvin = 1 / (a + (b * log(ThermistorResistance)) + (c * log(ThermistorResistance) * log(ThermistorResistance) * log(ThermistorResistance)));
  double Celsius = Kelvin - 273.0;
  double Fahrenheit = (Celsius * 9.0 / 5.0) + 32.0;        //Fahrenheit is now the actual temperature read by the thermometer
  
  //AmountFull is from 0-1 indicating current Temp within the specified Temp range
  double AmountFull = (Fahrenheit - MinTemp) / (MaxTemp - MinTemp);
  
  //When AmountFull says not enough leds are lit, light 1 more
  if (LedsLit + 1 <= AmountFull * LedCount) {
    LedBoard.setPixelColor(LedsLit, 255, 0, 0);
    LedBoard.show();
    LedsLit += 1;
  }
  
  //When AmountFull says too many leds are lit, turn 1 off
  if (LedsLit - 1 > AmountFull * LedCount) {
    LedsLit -= 1;
    LedBoard.setPixelColor(LedsLit, 0, 0, 0);
    LedBoard.show();
  }
  
  //Wait 100 ms before measuring again (10 Hz refresh rate)
  delay(100);
}

