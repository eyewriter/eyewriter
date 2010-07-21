/*
 StrobeEyePtGrey
 http://www.eyewriter.org/
 
 The strobe from a Point Grey camera drives the interrupts on pins 2 and 3
 (interrupts 0 and 1). When pin 2 is interrupted, the pupil LEDs are switched
 on and the glints are switched off. And when pin 3 is interrupted, the
 inverse occurs.
*/

/*
 Feel free to use different LED output pins.
*/
const int pupilPin = 11;
const int glintPin = 12;

/*
 The rest of the code shouldn't change between setups.
*/
const int interruptPupil = 0;
const int interruptPupilPin = 2;
const int interruptGlint = 1;
const int interruptGlintPin = 3;

void setup() {
  pinMode(pupilPin, OUTPUT);
  pinMode(glintPin, OUTPUT);
  attachInterrupt(interruptPupil, pupil, CHANGE);
  attachInterrupt(interruptGlint, glint, CHANGE);
}

void loop() {
}

void pupil() {
  digitalWrite(pupilPin, digitalRead(interruptPupilPin));
}

void glint() {
  digitalWrite(glintPin, digitalRead(interruptGlintPin));
}
