#include <LiquidCrystal.h> // Include LiquidCrystal library for LCD

const int buttonIncreasePin = 2;  // Pin connected to increase frequency button (Active-LOW)
const int buttonDecreasePin = 3;  // Pin connected to decrease frequency button (Active-LOW)
const int buttonStartPin = 4;     // Pin connected to start button (Active-LOW)
const int buttonStopPin = 5;      // Pin connected to stop button (Active-LOW)

const int pwmPin = 6;             // PWM output pin for VFD control
const int lcdRS = 13, lcdEn = 12, lcdD4 = 11, lcdD5 = 10, lcdD6 = 9, lcdD7 = 8;

LiquidCrystal lcd(lcdRS, lcdEn, lcdD4, lcdD5, lcdD6, lcdD7); // Initialize LCD

int previousFrequency = 50;
int originalFrequency = 50;
int resolution = 255;
int frequency = 50;            // Initial frequency (Hz)
bool isRunning = false;        // Flag to control signal generation

// Function to generate a triangular waveform
int triangle(int frequency) {
  const int resolution_ = 255; // PWM resolution (adjust based on your Arduino model)

  int dutyCycle = 0;          // Current duty cycle (0-resolution)
  bool isRampUp = true;       // Flag to indicate increasing or decreasing duty cycle

  // Update duty cycle based on ramp direction (similar logic from previous code)
  if (isRampUp) {
    dutyCycle++;
    if (dutyCycle == resolution_) {
      isRampUp = false;
    }
  } else {
    dutyCycle--;
    if (dutyCycle == 0) {
      isRampUp = true;
    }
  }

  // Set PWM value based on duty cycle
  analogWrite(pwmPin, dutyCycle);

  // Delay to control frequency
  delayMicroseconds(1000000 / frequency / 2); // Adjust delay for desired frequency
}

// Function to generate a sinusoidal waveform
void generateSineWave(int frequency) {
  unsigned long currentTime = micros();
  float angle = 2 * PI * frequency * currentTime / 1000000.0; // Calculate angle based on time and frequency

  // Generate PWM value using sin() and map()
  int pwmValue = int(map(long(sin(angle)),-1,1,0,255));

  // Set PWM duty cycle for the sine wave
  analogWrite(pwmPin, pwmValue);
}

// Function to debounce button press (works for all buttons)
bool debounce(int buttonPin) {
  if(!digitalRead(buttonPin))
  {
    delay(500);
    return (!digitalRead(buttonPin));
  }
  return false;
}

void setup() {
  // put your setup code here, to run once:
  // Configure button pins as inputs with internal pull-up resistors for active-LOW logic
  pinMode(buttonIncreasePin, INPUT_PULLUP);
  pinMode(buttonDecreasePin, INPUT_PULLUP);
  pinMode(buttonStartPin, INPUT_PULLUP);
  pinMode(buttonStopPin, INPUT_PULLUP);
  pinMode(pwmPin, OUTPUT);

  // changing PWM frequency on D6
  TCCR0B = TCCR0B & B11111000 | B00000010;  // for PWM frequency of 7812.50 Hz

  lcd.begin(16, 2);     // Initialize LCD with 16 columns and 2 rows
  lcd.clear();          // Clear LCD display
  lcd.print("Hello World!");
  delay(3000);
  lcd.clear();
  lcd.print("Press Start");
  isRunning = true;
  while(!debounce(buttonStartPin));
  lcd.clear();
  lcd.print("Frequency = ");  // Display "Freq: " on first line
  lcd.setCursor(0, 1);  // Set cursor to second line, first column
  lcd.print("f range 0 to 100"); // Display frequency range
  lcd.setCursor(14,0);   // set cursor to first line, 14th column
  lcd.print("Hz");      // Display hertz
  lcd.setCursor(12,0);   // set cursor to first line, 7th column
  lcd.print(frequency); // Display frequency value
}

void loop() {
  // put your main code here, to run repeatedly:
   // Read button states (active-LOW logic)
  bool increaseButtonPressed = !debounce(buttonIncreasePin);
  bool decreaseButtonPressed = !debounce(buttonDecreasePin);
  bool startButtonPressed = !debounce(buttonStartPin);
  bool stopButtonPressed = !debounce(buttonStopPin);

  // Handle button presses with input validation (optional)
  if (increaseButtonPressed && frequency < 100) {
    frequency++;
    // Update LCD display  
    lcd.setCursor(12,0);
    lcd.print(frequency); // Update frequency display
  }
  if (decreaseButtonPressed && frequency > 0) {
    frequency--;
    // Update LCD display
    lcd.setCursor(12,0);
    lcd.print(frequency); // Update frequency display
  }
  if (stopButtonPressed) {
    isRunning = false;
    analogWrite(pwmPin, 0);  // Stop PWM signal when stopped
  }

   // Generate PWM signal using a more robust sine wave generation method (replace with your implementation)
  if (isRunning) {
    // Implement a suitable sine wave generation algorithm here 
    // (e.g., using lookup tables, pre-calculated values, or mathematical functions)
    {
      if(0.9*sin(2*PI*frequency)>=triangle(frequency,micros())) {
        analogWrite(pwmPin,HIGH);
      }
      else {
        analogWrite(pwmPin,LOW);
      }
    }
    else {
      if(0.9*(frequency/originalFrequency)*sin(2*PI*frequency)>=triangle(frequency,micros())) {
        analogWrite(pwmPin,HIGH);
      }
      else {
        analogWrite(pwmPin,LOW);
      }
    }
  }
}
