#include <Arduino.h>
#include <math.h>
#include <LiquidCrystal.h>
LiquidCrystal lcd(13,12,11,10,9,8);

//const int sineFreq = 50; // Frequency of sine wave in Hz
const int sawtoothFreq = 55550; // Frequency of sawtooth wave in Hz
const int sampleInterval = 0.001; // Sampling interval in microseconds
const int sinePhaseShift = 180; // Phase shift of sine wave in degrees
const int outputPin1 = 5; // Output pin 1 for comparison result
const int outputPin2 = 6; // Output pin 2 for comparison result
float input_pin = A0;  // select ttimeone input pin for ttimeone potentiometer
float sineFreq;



void setup() {
      digitalWrite(outputPin1, HIGH);
    digitalWrite(outputPin2, HIGH);

  pinMode(outputPin1, OUTPUT);
  pinMode(outputPin2, OUTPUT);
}

void loop() {
  float ff = analogRead(input_pin);
  sineFreq=map(ff,0,1023,0,255);
  if(sineFreq<55 && sineFreq>45)
  {sineFreq=50;}
  else if (sineFreq>55)
  {sineFreq=60;}
  else if (sineFreq<45 && sineFreq>35)
  {sineFreq=40;}
  else if (sineFreq<35 && sineFreq>25)
  {sineFreq=30;}
  else if (sineFreq<25)
  {sineFreq=20;}
  else if (sineFreq==25)
  {sineFreq=sineFreq;}
  else if (sineFreq==35)
  {sineFreq=sineFreq;}
  else if (sineFreq==45)
  {sineFreq=sineFreq;}
  else if (sineFreq==55)
  {sineFreq=sineFreq;}

  lcd.clear();
  lcd.begin(16, 2);
  lcd.print("hello  world!");
  lcd.setCursor(0, 1);
  lcd.print(sineFreq);

 float com=sineFreq;
  while (com==sineFreq){
  float ff = analogRead(input_pin);
  sineFreq=map(ff,0,1023,0,255);
  if(sineFreq<55 && sineFreq>45)
  {sineFreq=50;}
  else if (sineFreq>55)
  {sineFreq=60;}
  else if (sineFreq<45 && sineFreq>35)
  {sineFreq=40;}
  else if (sineFreq<35 && sineFreq>25)
  {sineFreq=30;}
  else if (sineFreq<25)
  {sineFreq=20;}
  else if (sineFreq==25)
  {sineFreq=sineFreq;}
  else if (sineFreq==35)
  {sineFreq=sineFreq;}
  else if (sineFreq==45)
  {sineFreq=sineFreq;}
  else if (sineFreq==55)
  {sineFreq=sineFreq;}
  unsigned long startTime = micros(); // Get the current time in microseconds
  
 // Generate sine wave with 180 degree phase shift
  float sineValue = 0.4 * (1 + sin(2 * PI * sineFreq * (micros() / 1000000.0) ));

  // Generate sawtooth wave
  float sawtoothValue = ((micros() * sawtoothFreq) % 1000000) / 1000000.0;

  // Compare the values and handle negative sine value
  if (sineValue >= 0.425) {
    digitalWrite(outputPin1, HIGH);
    if (sawtoothValue > sineValue) {
    digitalWrite(outputPin2, HIGH); // Set output pin 2 high
  }
  else {
    digitalWrite(outputPin2, LOW); // Set output pin 2 low
   }}
   else if (sineValue <= 0.375) {
    digitalWrite(outputPin2, HIGH);
    if (sawtoothValue < (sineValue+0.1)) {
    digitalWrite(outputPin1, HIGH); // Set output pin 2 high
  }
  else {
    digitalWrite(outputPin1, LOW); // Set output pin 2 low
  }}
   else if (sineValue > 0.375 && sineValue<0.425) {
        digitalWrite(outputPin2, HIGH);
    digitalWrite(outputPin1, HIGH);}

  
  

  // Wait for the remaining time in the sampling interval
  unsigned long elapsedTime = micros() - startTime;
  if (elapsedTime < sampleInterval) {
    delayMicroseconds(sampleInterval - elapsedTime);
  }
  }}