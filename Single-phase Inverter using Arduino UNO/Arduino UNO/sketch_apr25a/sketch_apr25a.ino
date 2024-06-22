#include <LiquidCrystal.h>

const int increaseButton = 2;  // Pin connected to increase frequency button (Active-LOW)
const int decreaseButton = 3;  // Pin connected to decrease frequency button (Active-LOW)
const int stopButton = 4;      // Pin connected to stop button (Active-LOW)
const int startButton = 7;     // Pin connected to start button (Active-LOW)

const int pwmHigh = 6;  // PWM High side output pin for VFD control
const int pwmLow = 5;   // PWM Low side output pin for VFD control
const int lcdRS = 13, lcdEn = 12, lcdD4 = 11, lcdD5 = 10, lcdD6 = 9, lcdD7 = 8;

LiquidCrystal lcd(lcdRS, lcdEn, lcdD4, lcdD5, lcdD6, lcdD7);  // Initialize LCD

const int numReadings = 10;
int readings[numReadings];  // the readings from the analog input
int index = 0;              // the index of the current reading
int total = 0;              // the running total
int average = 0;            // the average

const int frequencyMin = 10;
const int frequencyMax = 60;
int frequency = 50;
int freqTriangle = 5000;
int freqError = 5;
unsigned long previousMillis = 0;
const long interval = 20;  //interval at which to blink (milliseconds)

volatile bool increaseFlag = false;
volatile bool decreaseFlag = false;

void setup() {
  pinMode(startButton, INPUT_PULLUP);
  pinMode(stopButton, INPUT_PULLUP);
  pinMode(increaseButton, INPUT_PULLUP);
  pinMode(decreaseButton, INPUT_PULLUP);
  pinMode(pwmHigh,OUTPUT);
  pinMode(pwmLow,OUTPUT);

  lcd.begin(16, 2);
  lcd.print("Hello World!");
  delay(3000);
  lcd.clear();
  lcd.print("Press Start");
  for (int thisReading = 0; thisReading < numReadings; thisReading++) {
    readings[thisReading] = 0;
  }
  
  attachInterrupt(digitalPinToInterrupt(increaseButton), increaseISR, FALLING);
  attachInterrupt(digitalPinToInterrupt(decreaseButton), decreaseISR, FALLING);
}

void loop() {
  static bool started = false;
  static bool stopped = false;
  static int prevFrequency = frequency;

  if (digitalRead(startButton) == LOW && !started) {
    started = true;
    stopped = false;
    lcd.clear();
    lcd.print("f = " + String(frequency) + " Hz");
    lcd.setCursor(0, 1);
    lcd.print("f range 10 to 60 Hz");
  }

  if (started) {
    unsigned long currentMillis = millis();

    if (increaseFlag) {
      increaseFlag = false;
      if (frequency < frequencyMax) {
        frequency++;
      }
    }

    if (decreaseFlag) {
      decreaseFlag = false;
      if (frequency > frequencyMin) {
        frequency--;
      }
    }

    if (currentMillis - previousMillis >= interval) {
      previousMillis = currentMillis;
      updatePWM(frequency);
    }

    if (digitalRead(stopButton) == LOW) {
      stopped = true;
      started = false;
      lcd.clear();
      frequency = 50;
      while (digitalRead(stopButton) == LOW);
    }

    if (frequency != prevFrequency) {
      lcd.setCursor(0, 0);
      lcd.print("f = " + String(frequency) + " Hz");
      prevFrequency = frequency;
    }
  }
}

void updatePWM(int frequency) {
  int amplitude = 128;  // Adjust this for desired output range (0-255)
  for (int i = 0; i < 256; i++) {
    // Calculate sine wave value (0 to peak-to-peak amplitude)
    float sineValue = amplitude * sin(2 * PI * frequency * millis() / 1000.0);
    float triangleValue ;
    // Calculate triangle wave value (peak-to-peak amplitude is 256)
    if ( sineValue > 0 ) { 
      triangleValue = triangularWave(5000.0 , 128 , 128);
    }
    else {
      triangleValue = triangularWave(5000.0 , 128 , 0);
    }

    // Compare sine and triangle wave values for PWM output
    int highDutyCycle = 0;
    int lowDutyCycle = 0;
    if (sineValue > triangleValue) {
      highDutyCycle = 255;
    } else {
      lowDutyCycle = 255;
    }

    analogWrite(pwmHigh, highDutyCycle);
    analogWrite(pwmLow, lowDutyCycle);
  }
}

void increaseISR() {
  increaseFlag = true;
}

void decreaseISR() {
  decreaseFlag = true;
}

float triangularWave(float frequency, float amplitude, float offset) {
  float phase = (millis() * frequency) / 1000.0; // calculate phase based on time and frequency
  phase = phase - floor(phase); // wrap phase to 0-1 range
  float value;
  if (phase < 0.5) {
    value = (phase / 0.5) * amplitude + offset;
  } else {
    value = (1 - (phase - 0.5) / 0.5) * amplitude + offset;
  }
  return value;
}