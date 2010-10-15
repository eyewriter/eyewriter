#pragma once

#include "ofMain.h"
#include "ofxOpenCv.h"
#include "baseVideoGrabber.h"
#include "threadedQtVideoGrabber.h"

class qtVideoGrabber : public baseVideoGrabber  {
	
public:
	void setup(int _width, int _height, bool _bUseTexture = false){
		vidGrabber.setup(_width, _height, this);
		width = vidGrabber.width;
		height = vidGrabber.height;
		
		if (width !=0 || height != 0){
			colorImg.setUseTexture(bUseTexture);
			grayImg.setUseTexture(bUseTexture);
			colorImg.allocate(width, height);
			grayImg.allocate(width, height);
		}
		vidGrabber.startCapture(false);
		
		bUseTexture = _bUseTexture;
		bNeedUpdate = false;
	}
	void update(){
		if (bNeedUpdate){
			vidGrabber.lock();
			bNeedUpdate = false;
			vidGrabber.unlock();
		}
	}
	void draw(int x, int y){
		if(bUseTexture){
			colorImg.draw(x,y);
		}
	}
	void setDeviceID(int device){
		vidGrabber.setDeviceID(device);
	}
	void openVideoSettings(){
		vidGrabber.lock();
		vidGrabber.videoSettings();
		vidGrabber.unlock();
	}
	
	
	//------- don't call this function from main thread, this is only for vidGrabber -------
	void frameReceived(ofVideoGrabber& grabber){
		vidGrabber.getThreadedPixels(colorImg, bNeedUpdate);
		vidGrabber.lock();															// lock again..
		cvCvtColor(colorImg.getCvImage(), grayImg.getCvImage(), CV_RGB2GRAY);
		vidGrabber.unlock();
	}
	
protected:
	threadedQtVideoGrabber	vidGrabber;
	ofxCvColorImage			colorImg;
	
};


