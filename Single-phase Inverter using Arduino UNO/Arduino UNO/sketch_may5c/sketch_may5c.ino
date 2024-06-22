const int pwm = 5;
const int triangle_period_us = 99; // Total period of the triangle wave (in microseconds)
const int ramp_up_time_us = 50; // Duration of the rising edge (in microseconds)

volatile byte triValue = 0;
volatile unsigned long lastTimeMicros;

void setup() {
  pinMode(pwm, OUTPUT);
  TCCR1A = 0;
  TCCR1B = B00000100; // Set prescaler to 250
  TCNT1 = 0;
  OCR1A = 50000; // Set compare match value to 50000
  TIMSK1 = 1;

  lastTimeMicros = micros();

}

ISR(TIMER1_COMPA_vect) {
  if (micros() - lastTimeMicros > triangle_period_us) {
    lastTimeMicros += triangle_period_us;
    triValue = 0;
  } 
  if (micros() - lastTimeMicros <= ramp_up_time_us) {
    triValue++;
  } else {
    triValue--;
  }
}

void loop() {
  analogWrite(pwm, triValue);
}