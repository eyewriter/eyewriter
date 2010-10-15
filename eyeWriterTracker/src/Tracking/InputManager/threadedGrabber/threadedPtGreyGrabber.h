#pragma once

#include "ofxLibdcPtGrey.h"
#include "ofxOpenCv.h"
#include "ofxThread.h"
#include "ofEvents.h"

#define INPUT_FIREFLY_WIDTH 752

class threadedPtGreyGrabber : public ofxLibdcPtGrey, public ofxThread {
protected:
	int pollRate;
	void threadedFunction() {
		while(threadRunning) {
			lock();
			update();			// we don't need to lock here if getThreadedPixels is called from "frameReceived"
			unlock();
			if(isFrameNew())
				ofNotifyEvent(frameReceived, *this);
			ofSleepMillis(pollRate);
		}
	}
	
public:
	
	ofEvent<ofxLibdcPtGrey> frameReceived;
	threadedPtGreyGrabber() {
		pollRate = 1;
	}
	template <class L>
	void setupGrabber(L* listener, bool useTexture = false, bool verbose = false) {
		addListener(listener);
		setUseTexture(useTexture);				
	}
	void startCapture( bool verbose = false){
		startThread(true, verbose);
	}
	void setPollRate(int pollRate) {
		this->pollRate = pollRate;
	}
	int getPollRate() {
		return pollRate;
	}
	template <class L>
	void addListener(L* listener) {
		ofAddListener(frameReceived, listener, &(L::frameReceived));
	}
	void getThreadedPixels(ofxCvGrayscaleImage & grayImg, bool & bNeedUpdate) {
		lock();
		grayImg.setFromPixels(this->getPixels(), width, height);
		bNeedUpdate = true;
		unlock();
	}
};
