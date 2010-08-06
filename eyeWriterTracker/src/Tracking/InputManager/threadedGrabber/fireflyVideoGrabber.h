#pragma once

#include "ofMain.h"
#include "ofxOpenCv.h"
#include "baseVideoGrabber.h"
#include "threadedPtGreyGrabber.h"

class fireflyVideoGrabber : public baseVideoGrabber {
	
public:
	
	void setup(int _width, int _height, bool _bUseTexture = false){
		
		bNeedUpdate = false;
		bUseTexture = _bUseTexture;
		
		//------------ setup fireflyMV ---------------
		vidGrabber.setSize(_width, _height);
		// the firefly sensor is 752x480, so center the ROI horizontally
		vidGrabber.setPosition((INPUT_FIREFLY_WIDTH - _width) / 2, 0);
		// ofxLibdc is currently grayscale only
		vidGrabber.setImageType(OF_IMAGE_GRAYSCALE);
		// use format 7 mode 0 (roi but no binning)
		vidGrabber.setFormat7(true);
		// setup firewire device 
		vidGrabber.setup(0);
		// set up the alternating strobe pattern for driving vsync
		// and embed the strobe info in the image
		vidGrabber.setupAlternatingStrobe();
		// query the camera for its max framerate at the current settings, and use that
		vidGrabber.setMaxFramerate();
		//-------------------------------------------
		
		width = (int)vidGrabber.getWidth();
		height = (int)vidGrabber.getHeight();
		
		if (width !=0 || height != 0){
			grayImg.setUseTexture(_bUseTexture);
			grayImg.allocate(width, height);
		}		
		vidGrabber.setupGrabber(this);
		vidGrabber.startCapture(false);
	}
	void update(){
		if (bNeedUpdate){
			vidGrabber.lock();
			bNeedUpdate = false;
			vidGrabber.unlock();
		}
	}
	void draw(int x, int y){
		if (bUseTexture){
			grayImg.draw(x,y);
		}
	}
	void setShutterNorm(float curShutter){
		vidGrabber.lock();
		vidGrabber.setShutterNorm(curShutter);
		vidGrabber.unlock();
	}
	void setGainNorm(float curGain){		
		vidGrabber.lock();
		vidGrabber.setGainNorm(curGain);
		vidGrabber.unlock();
	}
	void setBrightnessNorm(float curBrightness){
		vidGrabber.lock();
		vidGrabber.setBrightnessNorm(curBrightness);
		vidGrabber.unlock();
	}
	void setExposureNorm(float curExposure){
		vidGrabber.lock();
		vidGrabber.setExposureNorm(curExposure);
		vidGrabber.unlock();
	}
	void setGammaNorm(float curGamma){
		vidGrabber.lock();
		vidGrabber.setGammaNorm(curGamma);
		vidGrabber.unlock();
	}
	
	
	
	
	//------- don't call this function from main thread, this must be called only from threadedIIDCGrabber -------
	void frameReceived(ofxLibdcPtGrey& grabber){
		vidGrabber.getThreadedPixels(grayImg, bNeedUpdate);
	}
	
protected:
	
	threadedPtGreyGrabber	vidGrabber;	

};


