const int floorleds[] = {8, 9, 10};
const int Motion_Sensor_Pin = 11;
const int floor_buttons[] = {A0, A1, A2};
const int Speaker_Pin = 13;

int currentFloor = 0;
int targetFloor = 1;
bool elevatorMoving = false;
bool doorsClosed = true;
bool stationaryElevator = true;
unsigned long lastButtonPressTime = 0;
const unsigned long buttonDebounceDelay = 100;
unsigned long previousMillis = 0;
const long floorChangeDelay = 2000;
const long motionLightBlinkDelay = 600;
unsigned long previousMotionLightMillis = 0;
bool motionLightOn = LOW;

void setup() {
  for (int i = 0; i < 3; i++) {
    pinMode(floorleds[i], OUTPUT);
    pinMode(floor_buttons[i], INPUT_PULLUP);
  }
  pinMode(Motion_Sensor_Pin, OUTPUT);
  pinMode(Speaker_Pin, OUTPUT);
  setFloorIndicators(currentFloor);
}

void loop() {
  unsigned long currentTime = millis();
  for (int i = 0; i < 3; i++) {
    if (digitalRead(floor_buttons[i]) == LOW && (currentTime - lastButtonPressTime) > buttonDebounceDelay) {
      lastButtonPressTime = currentTime;
      if (i + 1 != currentFloor && doorsClosed) {
        targetFloor = i + 1;
        if (!elevatorMoving) {
          startElevator();
        }
      }
    }
  }

  if (elevatorMoving) {
    if (currentFloor == targetFloor) {
      stopElevator();
    } else {
      buzzSpeaker(400);
      setFloorIndicators(currentFloor);
      stationaryElevator = false;
      blinkMotionLight();
    }
  } else {
    digitalWrite(Motion_Sensor_Pin, LOW);
    if (stationaryElevator) {
      setFloorIndicators(currentFloor);
    }
  }
}

void startElevator() {
  elevatorMoving = true;
  doorsClosed = false;
  digitalWrite(Motion_Sensor_Pin, HIGH);
  digitalWrite(floorleds[currentFloor - 1], LOW);

  unsigned long prevMillis = millis();
  unsigned long changeDelay = 2000;

  while (currentFloor != targetFloor) {
    unsigned long currentTime = millis();
    if (currentFloor < targetFloor and (currentTime - prevMillis) >= changeDelay) {
      currentFloor++;
      prevMillis = currentTime;
    } else if (currentFloor > targetFloor and (currentTime - prevMillis) >= changeDelay) {
      currentFloor--;
      prevMillis = currentTime;
    }
    setFloorIndicators(currentFloor);
    buzzSpeaker(400);
    blinkMotionLight();
  }
  emitSound(1000);
  stopElevator();
}

void stopElevator() {
  digitalWrite(Motion_Sensor_Pin, LOW);
  elevatorMoving = false;
  doorsClosed = true;
  targetFloor = currentFloor;
  stationaryElevator = true;
  digitalWrite(floorleds[currentFloor - 1], HIGH);
}

void setFloorIndicators(int floor) {
  for (int i = 0; i < 3; i++) {
    if (i == floor - 1) {
      digitalWrite(floorleds[i], HIGH);
    } else {
      digitalWrite(floorleds[i], LOW);
    }
  }
}

void buzzSpeaker(int frequency) {
  tone(Speaker_Pin, frequency);
}

void emitSound(int frequency) {
  unsigned long currentTime = millis();
  unsigned long soundStartTime = currentTime;
  unsigned long soundDuration = 200;
  unsigned long pauseDuration = 200;
  unsigned long doorsClosingDuration = 200;
  buzzSpeaker(frequency);
  while (currentTime - soundStartTime < soundDuration) {
    currentTime = millis();
  }
  noTone(Speaker_Pin);
  soundStartTime = currentTime;
  while (currentTime - soundStartTime < pauseDuration) {
    currentTime = millis();
  }
  buzzSpeaker(1000);
  soundStartTime = currentTime;
  while (currentTime - soundStartTime < doorsClosingDuration) {
    currentTime = millis();
  }
  noTone(Speaker_Pin);
}

void blinkMotionLight() {
  unsigned long currentTime = millis();
  if (currentTime - previousMotionLightMillis >= motionLightBlinkDelay) {
    previousMotionLightMillis = currentTime;
    motionLightOn = (motionLightOn == LOW) ? HIGH : LOW;
    digitalWrite(Motion_Sensor_Pin, motionLightOn);
  }
}
