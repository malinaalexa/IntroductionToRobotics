// Define connections to the shift register and buttons
const int storagePin = 11; // Latch pin for the shift register
const int clockPin = 10;   // Clock pin for the shift register
const int dataPin = 12;    // Data pin for the shift register
const int segD1 = 4;       // Segment Digit 1
const int segD2 = 5;       // Segment Digit 2
const int segD3 = 6;       // Segment Digit 3
const int segD4 = 7;       // Segment Digit 4
const int startPauseBtn = A0; // Start/Pause button
const int resetBtn = A1;       // Reset button
const int lapBtn = A2;         // Lap button

int displayDigits[] = {segD1, segD2, segD3, segD4}; // Array of display digit pins
const int displayCount = 4;                          // Number of display digits
const int encodingsNumber = 16;                      // Number of possible encodings
byte byteEncodings[encodingsNumber] = {B11111100, B01100000, B11011010, B11110010, B01100110, B10110110, B10111110, B11100000, B11111110, B11110110, B11101110, B00111110, B10011100, B01111010, B10011110, B10001110};

unsigned long lastIncrement = 0; // Last time the number was incremented
unsigned long delayCount = 100;  // Delay for smoother operation
unsigned long number = 0;        // Current displayed number

int lapTimes[4] = {0, 0, 0, 0}; // Array to store lap times
int lapIndex = 0;               // Index for the current lap time
bool timerRunning = false;       // Flag indicating whether the timer is running
bool pauseMode = false;          // Flag indicating whether the timer is in pause mode
bool lastReset = true;           // Last state of the reset button

void setup() {
  pinMode(storagePin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);

  for (int i = 0; i < displayCount; i++) {
    pinMode(displayDigits[i], OUTPUT);
    digitalWrite(displayDigits[i], LOW);
  }

  pinMode(startPauseBtn, INPUT_PULLUP);
  pinMode(resetBtn, INPUT_PULLUP);
  pinMode(lapBtn, INPUT_PULLUP);

  Serial.begin(9600);
}

void loop() {
  handleButtons(); // Check and handle button presses

  // Increment the number if the timer is running and not in pause mode
  if (timerRunning && !pauseMode && millis() - lastIncrement > delayCount) {
    number++;
    number %= 10000; // Limit the number to four digits
    lastIncrement = millis();
  }
  writeNumber(number); // Display the current number

  // Display lap time if in pause mode and the reset button was just released
  if (lastReset == false && pauseMode) {
    writeNumber(lapTimes[lapIndex]);
  }
}

void handleButtons() {
  // Start/Pause button logic
  if (digitalRead(startPauseBtn) == LOW) {
    delay(50); // Debounce
    if (!timerRunning) {
      timerRunning = true;
      pauseMode = false;
    } else {
      timerRunning = false;
      pauseMode = true;
    }
    while (digitalRead(startPauseBtn) == LOW)
      ; // Wait for button release
  }

  // Reset button logic
  if (!timerRunning) {
    if (digitalRead(resetBtn) == LOW) {
      delay(50); // Debounce
      while (digitalRead(resetBtn) == LOW)
        ; // Wait for button release

      lastReset = !lastReset; // Toggle the last reset state

      // Reset lap index and lap times if reset button is pressed
      if (lastReset == true) {
        lapIndex = 0;
        memset(lapTimes, 0, sizeof(lapTimes));
        pauseMode = false;
      }

      number = 0;
      timerRunning = false;
      writeNumber(number); // Reset the display to "0000"
    }
  }

  // Lap button logic
  if (digitalRead(lapBtn) == LOW && timerRunning && !pauseMode) {
    delay(50); // Debounce
    // Save lap time and update lap index
    lapTimes[lapIndex] = number;
    lapIndex = (lapIndex + 1) % 4;
    writeNumber(lapTimes[lapIndex]); // Display the current lap time
    while (digitalRead(lapBtn) == LOW)
      ; // Wait for button release
  }
}

void writeReg(int digit) {
  digitalWrite(storagePin, LOW);
  shiftOut(dataPin, clockPin, MSBFIRST, digit);
  digitalWrite(storagePin, HIGH);
}

void activateDisplay(int displayNumber) {
  // Activate a specific display digit
  for (int i = 0; i < displayCount; i++) {
    digitalWrite(displayDigits[i], HIGH);
  }
  digitalWrite(displayDigits[displayNumber], LOW);
}

void writeNumber(int num) {
  int currentNum = num;
  int displayDigit = 3; // Start with the least significant digit
  int lastDigit = 0;

  // Loop through each display digit
  for (int i = 0; i < displayCount; i++) {
    lastDigit = currentNum % 10;
    activateDisplay(displayDigit);
    writeReg(byteEncodings[lastDigit]);
    delay(0); // A delay can be increased to visualize multiplexing
    displayDigit--;
    currentNum /= 10;
    writeReg(B00000000); // Clear the register to avoid ghosting
  }
}
