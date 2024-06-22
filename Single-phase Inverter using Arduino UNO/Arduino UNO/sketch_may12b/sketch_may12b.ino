#include <LiquidCrystal.h>

const int increaseButton = 2;  // Pin connected to increase frequency button (Active-LOW)
const int decreaseButton = 3;  // Pin connected to decrease frequency button (Active-LOW)
const int stopButton = 4;      // Pin connected to stop button (Active-LOW)
const int startButton = 7;     // Pin connected to start button (Active-LOW)

const int pwmHigh = 6;  // PWM High side output pin for VFD control
const int pwmLow = 5;   // PWM Low side output pin for VFD control
const int lcdRS = 13, lcdEn = 12, lcdD4 = 11, lcdD5 = 10, lcdD6 = 9, lcdD7 = 8;

LiquidCrystal lcd(lcdRS, lcdEn, lcdD4, lcdD5, lcdD6, lcdD7);  // Initialize LCD

const int frequencyMin = 10;
const int frequencyMax = 60;
int frequency = 50;

volatile bool increaseFlag = false;
volatile bool decreaseFlag = false;

// Define the variables for triangle wave generation
volatile int triangleValue = 0;
volatile int increment = 1;

// Setup timer
void setupTimer() {
  // Set timer1 interrupt at 15kHz
  TCCR1A = 0;
  TCCR1B = 0;
  TCNT1 = 0;
  OCR1A = 1000;  // Set compare match register for 15kHz
  TCCR1B |= (1 << WGM12);  // CTC mode
  TCCR1B |= (1 << CS10);   // No prescaler
  TIMSK1 |= (1 << OCIE1A); // Enable timer compare interrupt
}

                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                               // Timer1 compare match ISR
ISR(TIMER1_COMPA_vect) {
  
  // Increment or decrement triangle value
  if (triangleValue >= 255) {
    increment = -1;
  } else if (triangleValue <= 0) {
    increment = 1;
  }
  triangleValue += increment;
}

void setup() {
  lcd.begin(16, 2);
  lcd.print("Hello World!");
  delay(3000);
  lcd.clear();
  lcd.print("Press Start");
  pinMode(startButton, INPUT_PULLUP);
  pinMode(stopButton, INPUT_PULLUP);
  pinMode(increaseButton, INPUT_PULLUP);
  pinMode(decreaseButton, INPUT_PULLUP);
  // Setup timer interrupt
  setupTimer();

  TCCR0B = TCCR0B & B11111000 | B00000001;
  
  // Enable global interrupts
  sei();
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

  updatePWM(frequency);

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
  int highDutyCycle = 0;
  int lowDutyCycle = 0;
  int amplitude = 128;  // Adjust this for desired output range (0-255)
  for (int i = 0; i < 256; i++) {
    // Calculate sine wave value (0 to peak-to-peak amplitude)
    float sineValue = amplitude * sin(2 * PI * frequency * i / 256.0);

    // Compare sine and triangle wave values for PWM output
    if (sineValue > triangleValue) {
      highDutyCycle = 255;
      lowDutyCycle = 0;
    } else {
      highDutyCycle = 0;
      lowDutyCycle = 255;
    }

    analogWrite(pwmHigh, highDutyCycle);
    analogWrite(pwmLow, lowDutyCycle);
  }
}

void increaseISR() {
if (frequency < frequencyMax) {
      frequency++;
    }
}

void decreaseISR() {
if (frequency > frequencyMin) {
        frequency--;
      }
}