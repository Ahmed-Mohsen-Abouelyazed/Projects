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
int j = n / 2 + 1; // 180 degrees shifted from i

// Array to store duty cycle values
int dutycycle[510]; // hardcoded size

// Potentiometer pin
const int potPin = A0;

void setup() {
  // PIN DEFINITION
  pinMode(9, OUTPUT);
  pinMode(10, OUTPUT);

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
  OCR1B = dutycycle[j];

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
  float f_signal = (float)potValue / 1023.0 * 100.0;

  // Print the frequency value for debugging
  Serial.println(f_signal);

  // Wait for some time before reading the potentiometer again
  delay(10);
}

// ........................ TIMER 2 INTERRUPT SERVICE ROUTINE ................................
ISR(TIMER2_COMPA_vect) {
  // Update duty cycle values for pins 9 and 10
  if (i <= n - 1) {
    OCR1A = dutycycle[i];
    i++;
  } else {
    i = 0;
  }

  if (j <= n - 1) {
    OCR1B = dutycycle[j];
    j++;
  } else {
    j = 0;
  }
} // ISR