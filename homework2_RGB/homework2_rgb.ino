const int controlRPin=A0;
const int controlGPin=A1;
const int controlBPin=A2;
const int ledRPin=9;
const int ledGPin=10;
const int ledBPin=11;

int controlRValue=0;
int controlGValue=0;
int controlBValue=0;
int ledRValue=0;
int ledGValue=0;
int ledBValue=0;

void setup() {
  pinMode(controlRPin, INPUT);
  pinMode(controlGPin, INPUT);
  pinMode(controlBPin, INPUT);
  pinMode(ledRPin, OUTPUT);
  pinMode(ledGPin, OUTPUT);
  pinMode(ledBPin, OUTPUT);
}

void loop() {
  controlRValue=analogRead(controlRPin);
  controlGValue=analogRead(controlGPin);
  controlBValue=analogRead(controlBPin);
  ledRValue=map(controlRValue, 0, 1023, 0, 255);
  ledGValue=map(controlGValue, 0, 1023, 0, 255);
  ledBValue=map(controlBValue, 0, 1023, 0, 255);
  analogWrite(ledRPin, ledRValue);
  analogWrite(ledGPin, ledGValue);
  analogWrite(ledBPin, ledBValue);

}
