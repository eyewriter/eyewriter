/*
 ofxLibdcGrabber has all the features of ofxLibdc with the interface
 of ofBaseVideo. It uses the more traditional approach to capturing,
 which has the disadvantage of being slightly slower and requiring
 more memory -- but the advantage of being easy to swap out.
 
 ofxLibdcGrabber camera;
 ofImage currentFrame;
 camera.grabFrame();
 if(camera.isFrameNew()) {
 currentFrame.setFromPixels(camera.getPixels() ... );
 // do something with currentFrame
 }
 */

#pragma once

#include "ofxLibdc.h"

class ofxLibdcGrabber : public ofxLibdc, public ofBaseVideo {
public:
	ofxLibdcGrabber();
	ofTexture& getTextureReference();
	void setUseTexture(bool useTexture);
	unsigned char* getPixels();
	ofImage& getBuffer();
	void update();
	void grabFrame();
	void draw(float x, float y);
	void draw(float x, float y, float w, float h);
	float getWidth();
	float getHeight();
	bool isFrameNew();
	void close();
protected:
	ofImage buffer;
	bool newFrame;
};