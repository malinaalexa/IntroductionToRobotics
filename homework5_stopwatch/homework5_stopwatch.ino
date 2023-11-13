// Define connections to the shift register and buttons
const int latchPin = 11;
const int clockPin = 10;
const int dataPin = 12;
const int segD1 = 4;
const int segD2 = 5;
const int segD3 = 6;
const int segD4 = 7;
const int startPauseButton = A0;
const int resetButton = A1;
const int lapButton = A2;

int displayDigits[] = { segD1, segD2, segD3, segD4 };
const int displayCount = 4;
const int encodingsNumber = 16;
byte byteEncodings[encodingsNumber] = { B11111100, B01100000, B11011010, B11110010, B01100110, B10110110, B10111110, B11100000, B11111110, B11110110, B11101110, B00111110, B10011100, B01111010, B10011110, B10001110 };

unsigned long lastIncrement = 0;
unsigned long delayCount = 100; 
unsigned long number = 0;

int lapTimes[4] = { 0, 0, 0, 0 };
int lapIndex = 0;
bool timerRunning = false;
bool pauseMode = false;
int wasreset = false;

void setup() {
  // Set up pins for shift register and display digits
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
  
  for (int i = 0; i < displayCount; i++) {
    pinMode(displayDigits[i], OUTPUT);
    digitalWrite(displayDigits[i], LOW);
  }

  // Set up buttons with pull-up resistors
  pinMode(startPauseButton, INPUT_PULLUP);
  pinMode(resetButton, INPUT_PULLUP);
  pinMode(lapButton, INPUT_PULLUP);

  // Initialize serial communication for debugging
  Serial.begin(9600);
}

void loop() {
  // Check and handle button presses
  handleButtons();

  // Increment the timer if running and not in pause mode
  if (timerRunning && !pauseMode && millis() - lastIncrement > delayCount) {
    number++;
    number %= 10000;
    lastIncrement = millis();
  }
  
  // Display the current number on the 7-segment display
  writeNumber(number);
  /*if (pauseMode && wasreset == false) {
    writeNumber(lapTimes[lapIndex]);
  } this part of the code is broken*/ 
}

void handleButtons() {
  // Handle start/pause button press
  if (digitalRead(startPauseButton) == LOW) {
    delay(50); // debounce
    if (!timerRunning) {
      timerRunning = true;
      pauseMode = false;
    } else {
      timerRunning = false;
      pauseMode = true;
    }
    while (digitalRead(startPauseButton) == LOW)
      ; // wait for button release
  }

  // Handle reset button press when the timer is not running
  if (!timerRunning) {
    if (digitalRead(resetButton) == LOW) {
      delay(50); // debounce
      while (digitalRead(resetButton) == LOW)
        ; // wait for button release

      // Reset the timer and lap times
      wasreset = true;
      number = 0;
      lapIndex = 0;
      memset(lapTimes, 0, sizeof(lapTimes));
      timerRunning = false;
      pauseMode = false;
      writeNumber(number); // Reset the display to "0000"
    }
  }

  // Handle lap button press during the running timer and not in pause mode
  if (digitalRead(lapButton) == LOW && timerRunning && !pauseMode) {
    delay(50); // debounce
    // Save lap time
    lapTimes[lapIndex] = number;
    lapIndex = (lapIndex + 1) % 4;
    while (digitalRead(lapButton) == LOW)
      ; // wait for button release
  } else if (digitalRead(lapButton) == LOW && pauseMode) {
    delay(50); // debounce
    // Cycle through lap times in pause mode
    lapIndex = (lapIndex + 1) % 4;
    while (digitalRead(lapButton) == LOW)
      ; // wait for button release
  }
}

void writeReg(int digit) {
  // Write a digit to the shift register
  digitalWrite(latchPin, LOW);
  shiftOut(dataPin, clockPin, MSBFIRST, digit);
  digitalWrite(latchPin, HIGH);
}

void activateDisplay(int displayNumber) {
  // Activate a specific 7-segment display
  for (int i = 0; i < displayCount; i++) {
    digitalWrite(displayDigits[i], HIGH);
  }
  digitalWrite(displayDigits[displayNumber], LOW);
}

void writeNumber(int number) {
  // Display a 4-digit number on the 7-segment display
  int currentNumber = number;
  int displayDigit = 3; // Start with the least significant digit
  int lastDigit = 0;

  for (int i = 0; i < displayCount; i++) {
    lastDigit = currentNumber % 10;
    activateDisplay(displayDigit);
    writeReg(byteEncodings[lastDigit]);
    delay(0); // A delay can be increased to visualize multiplexing
    displayDigit--;
    currentNumber /= 10;
    writeReg(B00000000); // Clear the register to avoid ghosting
  }
}
