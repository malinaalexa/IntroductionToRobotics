#include "LedControl.h" // Include the LedControl library for controlling LED matrices
// Pin definitions for the LED matrix
const int dinPin = 12;
const int clockPin = 11;
const int loadPin = 10;
// Pin definitions for the joystick
const int xPin = A0;
const int yPin = A1;
const int swPin = A2;
// Create an LedControl object to interface with the LED matrix
LedControl lc = LedControl(dinPin, clockPin, loadPin, 1); // DIN, CLK, LOAD, number of devices
// Variables to control LED matrix brightness and position
byte matrixBrightness = 2;
byte xPos = 0;
byte yPos = 0;
byte xLastPos = 0;
byte yLastPos = 0;
// Thresholds for joystick movement detection
const int minThreshold = 200;
const int maxThreshold = 600;
// Timing variables for movement updates
const byte moveInterval = 500;
unsigned long lastMoved = 0;
// Size of the LED matrix (8x8)
const byte matrixSize = 8;
bool matrixChanged = true;
unsigned long swPressTime = 0;
byte swState = LOW;
byte lastSwState = HIGH;

// 2D array representing the state (on/off) of each LED in the matrix
byte matrix[matrixSize][matrixSize] = { /* Initial state with all LEDs off */ };

int explodeTime = 1000;//one second to explode

void generateRandomWalls(byte targetMatrix[matrixSize][matrixSize]) {
  for (int row = 0; row < matrixSize; row++) {
    for (int col = 0; col < matrixSize; col++)
    if(row==0 && col ==0 || row==1 && col==0 || row==0 && col==1 || row==1 && col==1) {
      //dont generate on the starting point for the player and on the neighbour steps. i did this so that the player isn't stuck 
    }
    else
    {
      // Generate random values (0 or 1)
      targetMatrix[row][col] = random(2);
}}}

void setup() {
pinMode(swPin, INPUT_PULLUP);

lc.shutdown(0, false); // Disable power saving, turn on the display
lc.setIntensity(0, matrixBrightness); // Set the brightness level
lc.clearDisplay(0); // Clear the display initially
matrix[xPos][yPos] = 1; // Turn on the initial LED position

generateRandomWalls(matrix);
}

void loop() {
// Check if it's time to move the LED
if (millis() - lastMoved > moveInterval) {
updatePositions(); // Update the LED position based on joystick input
lastMoved = millis(); // Reset the movement timer
}
// Update the LED matrix display if there's been a change
if (matrixChanged) {
updateMatrix();
matrixChanged = false;
}

lc.setLed(0, xPos, yPos, millis() % 1000 < 250);//make the player blink slowly every second

swState = digitalRead(swPin);
  if (swState != lastSwState) {
    if (swState == LOW) {
      swPressTime = millis(); 
      explodeTime = millis() + 2000;//explode in 2 seconds
// if the button was pressed we measure how long it is pressed using millis
    } 
    else {
      if (millis() - swPressTime < 1000) {//this is so that nothing happens when you keep the button pressed
      placebomb(xPos,yPos);
      updateMatrix();
      } 
    }
    lastSwState = swState;
  }

for (int row = 0; row < matrixSize; row++) 
for (int col = 0; col < matrixSize; col++)
if(matrix[row][col]==2){
lc.setLed(0, row, col, millis() % 200 < 250);
}//blink the bomb fast. not really necessary since i already do it in update matrix but i am too afraid to break it so I wont remove it
}

// Function to update the LED matrix display
void updateMatrix() {
  for (int row = 0; row < matrixSize; row++) {
    for (int col = 0; col < matrixSize; col++) {
      if (matrix[row][col] == 2) {
        if (millis() < explodeTime) {
          // Blink the bomb fast
          lc.setLed(0, row, col, millis() % 200 < 100);
        } else {
          // Explode the bomb and turn off the LED
          matrix[row][col] = 0;
          lc.setLed(0, row, col, false);
          matrixChanged = true; // Matrix has changed due to bomb explosion
        }
      } else {
        // Update other LEDs
        lc.setLed(0, row, col, matrix[row][col]);
      }
    }
  }
}

// Function to read joystick input and update LED positions accordingly
void updatePositions() {
int xValue = analogRead(xPin); // Read the X-axis value
int yValue = analogRead(yPin); // Read the Y-axis value
// Store the last positions
xLastPos = xPos;
yLastPos = yPos;

// Update xPos based on joystick movement
if (xValue < minThreshold && matrix[(xPos + 1) % matrixSize][yPos]!=1) {
xPos = (xPos + 1) % matrixSize;
} else if (xValue > maxThreshold && matrix[xPos - 1][yPos]!=1) {
xPos = (xPos > 0) ? xPos - 1 : matrixSize - 1;
}
// Update yPos based on joystick movement
if (yValue < minThreshold && matrix[xPos][(yPos - 1) % matrixSize]!=1) {
yPos = (yPos > 0) ? yPos - 1 : matrixSize - 1;
} else if (yValue > maxThreshold && matrix[xPos][(yPos + 1) % matrixSize]!=1) {
yPos = (yPos + 1) % matrixSize;
}


  
// Check if the position has changed and update the matrix accordingly
if (xPos != xLastPos || yPos != yLastPos) {
matrixChanged = true;
matrix[xLastPos][yLastPos] = 0; // Turn off the LED at the last position
matrix[xPos][yPos] = 1; // Turn on the LED at the new position
}
}


void placebomb(int x, int y) {
// Update the player. The bomb is placed in the current position of the player so they player is moved in an available neighbour empty space

if (matrix[(xPos + 1) % matrixSize][yPos]!=1) {
xPos = (xPos + 1) % matrixSize;
} else if (matrix[xPos - 1][yPos]!=1) {
xPos = (xPos > 0) ? xPos - 1 : matrixSize - 1;
}
else
if (matrix[xPos][(yPos - 1) % matrixSize]!=1) {
yPos = (yPos > 0) ? yPos - 1 : matrixSize - 1;
} else if (matrix[xPos][(yPos + 1) % matrixSize]!=1) {
yPos = (yPos + 1) % matrixSize;
}
if (xPos != xLastPos || yPos != yLastPos) {
matrixChanged = true;
matrix[xLastPos][yLastPos] = 2; // place bomb
matrix[xPos][yPos] = 1; // Turn on the LED at the new position
updateMatrix();
}

unsigned long startTime = millis();

  // Update the matrix to place the bomb
  matrix[x][y] = 2; // Use a different value to represent the bomb (e.g., 2)

  while (millis() - startTime < 2000) {
    lc.setLed(0, x, y, millis() % 200 < 100); // Blink the bomb LED
  }

  // Explode the bomb and update the matrix
  matrix[x + 1][y] = 0;
  matrix[x - 1][y] = 0;
  matrix[x][y + 1] = 0;
  matrix[x][y - 1] = 0;
  matrix[x][y] = 0; // Turn off the LED at the bomb position
  matrixChanged = true; // Matrix has changed due to bomb explosion
  explodeTime = 0;


}
