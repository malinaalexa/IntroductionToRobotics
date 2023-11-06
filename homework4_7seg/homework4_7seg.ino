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
    ledState = !ledState;
  }
  for (int i = 0; i < numSegments; i++) {
    digitalWrite(segmentsPins[i], LOW);
  }
  activeSegmentPin = segmentDPPin;
  digitalWrite(activeSegmentPin, HIGH);
}

void loop() {
  xJoystickValue = analogRead(joyXPin);
  yJoystickValue = analogRead(joyYPin);
  if (ActiveSegon_off == true) {
    OnOff(activeSegmentPin, lastONOFFMillis, ONOFFInterval);
  } 
  else 
    digitalWrite(activeSegmentPin, HIGH);

  if (xJoystickValue < minJoystickThreshold && !isJoystickMoved) {
    activeSegmentPin = nextSegment(activeSegmentPin, 'L');
    isJoystickMoved = true;
  } 
  else if (xJoystickValue > maxJoystickThreshold && !isJoystickMoved) {
    activeSegmentPin = nextSegment(activeSegmentPin, 'R');
    isJoystickMoved = true;
  }

  if (yJoystickValue < minJoystickThreshold && !isJoystickMoved) {
    activeSegmentPin = nextSegment(activeSegmentPin, 'U');
    isJoystickMoved = true;
  } 
  else if (yJoystickValue > maxJoystickThreshold && !isJoystickMoved) {
    activeSegmentPin = nextSegment(activeSegmentPin, 'D');
    isJoystickMoved = true;
  }

  if (xJoystickValue >= minJoystickThreshold && xJoystickValue <= maxJoystickThreshold && yJoystickValue >= minJoystickThreshold && yJoystickValue <= maxJoystickThreshold)
    isJoystickMoved = false;

  buttonState = digitalRead(switchPin);
  if (buttonState != lastButtonState) {
    if (buttonState == LOW) {
      buttonPressTime = millis();
    } 
    else {
      if (millis() - buttonPressTime < 1000) {
        isButtonPressed = !isButtonPressed;
        ActiveSegon_off = false;
      } 
      else {
        for (int i = 0; i < numSegments; i++)
          digitalWrite(segmentsPins[i], LOW);
        activeSegmentPin = segmentDPPin;
        ActiveSegon_off = true;
        digitalWrite(activeSegmentPin, HIGH);
      }
    }
    lastButtonState = buttonState;
  }

  if (ActiveSegon_off == true) {
    OnOff(activeSegmentPin, lastONOFFMillis, ONOFFInterval);
  } 
  else  digitalWrite(activeSegmentPin, HIGH);
}

void OnOff(int segment, unsigned long &lastONOFFMillis, unsigned long ONOFFInterval) {
  unsigned long currentMillis = millis();
  if (currentMillis - lastONOFFMillis >= ONOFFInterval) {
    lastONOFFMillis = currentMillis;
    int ledState = digitalRead(segment);
    digitalWrite(segment, !ledState);
  }
}

int nextSegment(int segment, char dir) {
  if (ActiveSegon_off == true)
    digitalWrite(activeSegmentPin, LOW);
  else ActiveSegon_off = true;

  if (dir == 'U') {
    if (segment == segmentsPins[1] || segment == segmentsPins[5] || segment == segmentsPins[6]) {
      return segmentsPins[0];
    } 
    else if (segment == segmentsPins[2] || segment == segmentsPins[3] || segment == segmentsPins[4]) {
      return segmentsPins[6];
    } 
    else return segment;

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
