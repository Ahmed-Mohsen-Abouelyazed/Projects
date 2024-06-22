#include <LiquidCrystal.h>

// Frequency of the PWM
const int f_pwm = 5050; // Hz

// Top value for the resolution selected (calculated from f_pwm, but hardcoded to 255)
const int TOP = (int)(16000000 / (f_pwm * 2) - 1);

// Length of the array (calculated from TOP)
const int n = TOP * 2;

// Sampling frequency (not used in this code)
// const int fs = n / 0.02; // Hz

// Cursors for the duty cycle array
int i = 0;

// Array to store duty cycle values
int dutycycle[510]; // hardcoded size

// Potentiometer pin
const int potPin = A0;

const int lcdRS = 12, lcdEn = 11, lcdD4 = 5, lcdD5 = 4, lcdD6 = 3, lcdD7 = 2;

LiquidCrystal lcd(lcdRS, lcdEn, lcdD4, lcdD5, lcdD6, lcdD7);  // Initialize LCD

void setup() {
  // PIN DEFINITION
  pinMode(9, OUTPUT);
  pinMode(10, OUTPUT);

  lcd.begin(16, 2);
  lcd.print("Hello World!");
  delay(3000);
  lcd.clear();

  // .................................. DUTYCYCLE COMPUTATION ................................................
  for (int i = 0; i <= n; i++) {
    // Calculate duty cycle value using sine function
    dutycycle[i] = (int)((1 + sin(2 * PI * i / n)) * (TOP / 2) + 0.5);
  }

  // .................................. TIMER 1: PWM ..................................................................
  cli(); // Deactivate global interrupts
  TCNT1 = 0; // Initialize counter value to zero

  // Configure Timer 1 for fast PWM mode
  TCCR1A = (1 << COM1A1) | (1 << COM1B1) | (1 << WGM11);
  TCCR1B = (1 << WGM13) | (1 << WGM12) | (1 << CS10);

  // Set overflow value for PWM resolution
  ICR1 = TOP;

  // Set initial duty cycle values for pins 9 and 10
  OCR1A = dutycycle[i];
  OCR1B = !dutycycle[i];

  // .................................. TIMER 2: INTERRUPT ..................................................
  TCCR2A = 0; // Reset Timer 2 registers
  TCCR2B = 0;

  // Configure Timer 2 for comparing output mode
  TCCR2A |= (1 << WGM21);
  TCCR2B |= (1 << CS21); // Prescaler at 8

  // Set OCR2A value for interrupt frequency
  OCR2A = 78; // hardcoded value

  TIMSK2 |= (1 << OCIE2A); // Enable Timer 2 Output Compare Match A Interrupt
  sei(); // Activate global interrupts
} // setup

void loop() {
  // Read the value of the potentiometer
  int potValue = analogRead(potPin);

  // Calculate the frequency of the sine wave
  int frequency = int(map(potValue, 0, 1023, 10, 60));

  lcd.print("f = ");
  lcd.print(frequency);
  lcd.print(" Hz");
  lcd.setCursor(0, 1);
  lcd.print("f range 10 to 60 Hz");

}

// ........................ TIMER 2 INTERRUPT SERVICE ROUTINE ................................
ISR(TIMER2_COMPA_vect) {
  // Update duty cycle values for pins 9 and 10
  if (i <= n - 1) {
    OCR1A = dutycycle[i];
    OCR1B = !dutycycle[i];
    i++;
  } else {
    i = 0;
  }
}