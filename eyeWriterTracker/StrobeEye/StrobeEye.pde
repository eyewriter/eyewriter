/*
 StrobeEye
 http://www.eyewriter.org/
 
 The strobe (also called vertical sync) from a camera is broken out and
 drives an interrupt on pin 3 (interrupt 1). The loop() uses the offset
 defined by vsync() to determine whether the LEDs on pin 11 or the LEDs
 on pin 12 should be on.
*/

/*
 This should be set to the same fps you expect the camera to capture at.
*/
const float fps = 30;

/*
 If you are using a PS3Eye or other rolling shutter camera, you also
 need to set the exposure. An exposure value of 1 means the camera is
 exposing half the sensor at any given moment, and an exposure value of
 0 means the sensor is exposing only one row at any given moment.
*/
const float exposure = 1;

/*
 For nicer cameras with global shutters, define USE_GLOBAL_SHUTTER.
 This will disable the use of the exposure variable, and leave the LEDs
 on as long as possible.
*/
//#define USE_GLOBAL_SHUTTER

/*
 Most cameras have a strobe signal that is tied to HIGH, and brought to
 LOW when a frame is captured. If you are buffering the signal with a
 transistor, this will invert the behavior and the polarity of the change
 will probably be RISING. If you are tapping the strobe directly, the
 polarity will probably be FALLING.
*/
const int polarity = RISING;

/*
 Feel free to use different LED output pins.
*/
const int glintPin = 11;
const int pupilPin = 12;

/*
 The rest of the code shouldn't change between setups.
*/
const int interrupt = 1;
volatile unsigned long offset = 0; 
volatile boolean firstFrame = true;
const float microScale = 1000000;
const float totalTime = microScale / fps;
const float cutoff = 1 - (exposure / 2);

float position;

void setup() {
 pinMode(pupilPin, OUTPUT);
 pinMode(glintPin, OUTPUT);
 attachInterrupt(interrupt, vsync, polarity);
}

void loop() {
 position = (float) (micros() - offset) / totalTime;
 #ifdef USE_GLOBAL_SHUTTER
  if(position < 1) {
   digitalWrite(pupilPin, HIGH);
   digitalWrite(glintPin, LOW);
 } else {
   digitalWrite(pupilPin, LOW);
   digitalWrite(glintPin, HIGH);
 }
 #else
 if(position < cutoff) {
   digitalWrite(pupilPin, HIGH);
   digitalWrite(glintPin, LOW);
 } else if (position > 1 && position - 1 < cutoff) {
   digitalWrite(pupilPin, LOW);
   digitalWrite(glintPin, HIGH);
 } else {
   digitalWrite(pupilPin, LOW);
   digitalWrite(glintPin, LOW);
 }
 #endif
}

void vsync() {
 firstFrame = !firstFrame;
 if(firstFrame)
   offset = micros();
}
