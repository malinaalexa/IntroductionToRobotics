const int switchPin = 2;
const int joyXPin = A0;
const int joyYPin = A1;

const int segmentAPin = 12;
const int segmentBPin = 10;
const int segmentCPin = 9;
const int segmentDPin = 8;
const int segmentEPin = 7;
const int segmentFPin = 6;
const int segmentGPin = 5;
const int segmentDPPin = 4;

const int numSegments = 8;

bool CommonAnode = false;
byte ledState = HIGH;
bool isJoystickMoved = false;
int minJoystickThreshold = 400;
int maxJoystickThreshold = 600;
int xJoystickValue = 0;
int yJoystickValue = 0;
byte buttonState = LOW;
byte lastButtonState = LOW;
int activeSegmentPin = segmentDPPin;
bool isButtonPressed = false;
unsigned long lastONOFFMillis = 0;
const unsigned long ONOFFInterval = 500;
unsigned long buttonPressTime = 0;
bool isButtonLongPressed = false;
bool ActiveSegon_off = true;

int segmentsPins[numSegments] = {
  segmentAPin, segmentBPin, segmentCPin, segmentDPin, segmentEPin, segmentFPin, segmentGPin, segmentDPPin};

bool segmentsState[numSegments] = {
  LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW};

void setup() {
  for (int i = 0; i < numSegments; i++) {
    pinMode(segmentsPins[i], OUTPUT);
  }
  pinMode(switchPin, INPUT_PULLUP);
  if (CommonAnode == true) {
    ledState = !ledState; // revert the setuo for common anode
  }
  for (int i = 0; i < numSegments; i++) {
    digitalWrite(segmentsPins[i], LOW);
  }
  activeSegmentPin = segmentDPPin; //starting from db
  digitalWrite(activeSegmentPin, HIGH);
}

void loop() {
  xJoystickValue = analogRead(joyXPin); //read the movement of the joystick on the X Y axis
  yJoystickValue = analogRead(joyYPin);


  if (xJoystickValue < minJoystickThreshold && !isJoystickMoved) {
    activeSegmentPin = nextSegment(activeSegmentPin, 'L'); //if it moved on the X axis to the left we go to that segment
    isJoystickMoved = true;
  } 
  else if (xJoystickValue > maxJoystickThreshold && !isJoystickMoved) {
    activeSegmentPin = nextSegment(activeSegmentPin, 'R'); //if it moved on the X axis to the right we go to that segment
    isJoystickMoved = true;
  }

  if (yJoystickValue < minJoystickThreshold && !isJoystickMoved) {
    activeSegmentPin = nextSegment(activeSegmentPin, 'U'); //if it moved on the Y axis up we go to that segment
    isJoystickMoved = true;
  } 
  else if (yJoystickValue > maxJoystickThreshold && !isJoystickMoved) {
    activeSegmentPin = nextSegment(activeSegmentPin, 'D');//if it moved on the Y axis down we go to that segment
    isJoystickMoved = true;
  }

  if (xJoystickValue >= minJoystickThreshold && xJoystickValue <= maxJoystickThreshold && yJoystickValue >= minJoystickThreshold && yJoystickValue <= maxJoystickThreshold)
    isJoystickMoved = false;

  buttonState = digitalRead(switchPin);
  if (buttonState != lastButtonState) {
    if (buttonState == LOW) {
      buttonPressTime = millis(); // if the button was pressed we measure how long it is pressed using millis
    } 
    else {
      if (millis() - buttonPressTime < 1000) {//if it was a short press
        isButtonPressed = !isButtonPressed;
        ActiveSegon_off = false; //the segment should be on, without flickering
      } 
      else {
        for (int i = 0; i < numSegments; i++)
          digitalWrite(segmentsPins[i], LOW);//reset to the db pin
        activeSegmentPin = segmentDPPin;
        ActiveSegon_off = true;
        digitalWrite(activeSegmentPin, HIGH);
      }
    }
    lastButtonState = buttonState;
  }

  if (ActiveSegon_off == true) {
    OnOff(activeSegmentPin, lastONOFFMillis, ONOFFInterval); //if the segment is selected and it should be flickering we do that
  } 
  else 
    digitalWrite(activeSegmentPin, HIGH); // otherwise (if the button was pressed) just let the segment be on
}

void OnOff(int segment, unsigned long &lastONOFFMillis, unsigned long ONOFFInterval) {
  unsigned long currentMillis = millis();
  if (currentMillis - lastONOFFMillis >= ONOFFInterval) {
    lastONOFFMillis = currentMillis; // measure how long to stay on and off
    int ledState = digitalRead(segment);
    digitalWrite(segment, !ledState);
  }
}

int nextSegment(int segment, char dir) {
  if (ActiveSegon_off == true)
    digitalWrite(activeSegmentPin, LOW); //keep the segmennt off if we moved and the button wasn't pressed
  else ActiveSegon_off = true;
  //Select the specific segmnent for each possible movement
  if (dir == 'U') {
    if (segment == segmentsPins[1] || segment == segmentsPins[5] || segment == segmentsPins[6]) {
      return segmentsPins[0];
    } 
    else if (segment == segmentsPins[2] || segment == segmentsPins[3] || segment == segmentsPins[4]) {
      return segmentsPins[6];
    } 
    else return segment; // if there is no neighbour segment, we stay on the current one

  } else if (dir == 'D') {
    if (segment == segmentsPins[0] || segment == segmentsPins[1] || segment == segmentsPins[5]) {
      return segmentsPins[6];
    } 
    else if (segment == segmentsPins[2] || segment == segmentsPins[4] || segment == segmentsPins[6]) {
      return segmentsPins[3];
    } 
    else return segment;

  } else if (dir == 'L') {
    if (segment == segmentsPins[0] || segment == segmentsPins[1]) {
      return segmentsPins[5];
    } 
    else if (segment == segmentsPins[2] || segment == segmentsPins[3]) {
      return segmentsPins[4];
    } 
    else if (segment == segmentsPins[7]) {
      return segmentsPins[2];
    } 
    else return segment;

  } else if (dir == 'R') {
    if (segment == segmentsPins[0] || segment == segmentsPins[5]) {
      return segmentsPins[1];
    } 
    else if (segment == segmentsPins[2]) {
      return segmentsPins[7];
    } 
    else if (segment == segmentsPins[3] || segment == segmentsPins[4]) {
      return segmentsPins[2];
    } 
    else return segment;

  }}
