#include "ofxLibdcGrabber.h"

ofxLibdcGrabber::ofxLibdcGrabber() :
	newFrame(false) {
}

ofTexture& ofxLibdcGrabber::getTextureReference() {
	return buffer.getTextureReference();
}

void ofxLibdcGrabber::setUseTexture(bool useTexture) {
	buffer.setUseTexture(useTexture);
}

unsigned char* ofxLibdcGrabber::getPixels() {
	return buffer.getPixels();
}

ofImage& ofxLibdcGrabber::getBuffer() {
	return buffer;
}

void ofxLibdcGrabber::update() {
	grabFrame();
}

void ofxLibdcGrabber::grabFrame() {
	newFrame = grabVideo(buffer);
}

void ofxLibdcGrabber::draw(float x, float y) {
	buffer.draw(x, y);
}

void ofxLibdcGrabber::draw(float x, float y, float w, float h) {
	buffer.draw(x, y, w, h);
}

float ofxLibdcGrabber::getWidth() {
	return ofxLibdc::getWidth();
}

float ofxLibdcGrabber::getHeight() {
	return ofxLibdc::getHeight();
}

bool ofxLibdcGrabber::isFrameNew() {
	if(newFrame) {
		newFrame = false;
		return true;
	}
	return false;
}

void ofxLibdcGrabber::close() {
	// handled by ~ofxLibdc
}
