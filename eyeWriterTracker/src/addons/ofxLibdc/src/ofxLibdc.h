/*
 ofxLibdc is a wrapper for libdc1394 that makes it easy to grab images from
 libdc1394-compatible cameras.
 
 It avoids unnecessary threading by using libdc1394's non-blocking image
 grabbing, which means you can call grabVideo without worrying about it slowing
 down your application.
 
 Instead of copying the image to an internal buffer before giving it to you,
 ofxLibdc will fill the image you pass to it. This avoids unnecessary copying.
 
 Grabbing with ofxLibdc looks like:
 
 ofLibdc camera;
 ofImage currentFrame;
 if(camera.grabVideo(currentFrame)) {
	// do something with currentFrame
 }
 
 Because there is no separate capture thread, there is no overhead from copying
 images you don't need.
 
 ofxLibdc can dynamically changed a number of parameters. setPosition() can be
 used to change the ROI position without restarting the camera. Other changes
 can be made, but will cause slight delays. Format 7 can be switched on and off,
 or between modes, 1394b can be switched on and off, and the ROI can be resized.
 
 grabStill uses a special mode present on many cameras called "one-shot mode"
 that allows the camera to capture a single frame.
 */
#pragma once

#include "ofMain.h"
#include "dc1394.h"

#define OFXLIBDC_BUFFER_SIZE 4

class ofxLibdc {
public:
	ofxLibdc();
	static int getCameraCount();
	
	void setSize(unsigned int width, unsigned int height);
	void setPosition(unsigned int roiLeft, unsigned int roiTop);
	void setImageType(int imageType);
	void setFormat7(bool useFormat7, int mode = 0);
	void set1394b(bool use1394b);
	void setBlocking(bool blocking);
	
	virtual void setup(int cameraNumber = 0);
	virtual ~ofxLibdc();
	
	void setBrightness(unsigned int brightness);
	void setGamma(unsigned int gamma);
	void setGain(unsigned int gain);
	void setExposure(unsigned int exposure);
	void setShutter(unsigned int shutter);
	void setFeature(dc1394feature_t feature, unsigned int value);
	
	void setBrightnessNorm(float brightness);
	void setGammaNorm(float gamma);
	void setGainNorm(float gain);
	void setExposureNorm(float exposure);
	void setShutterNorm(float shutter);
	void setFeatureNorm(dc1394feature_t feature, float value);
	
	void getBrightnessRange(unsigned int* min, unsigned int* max) const;
	void getGammaRange(unsigned int* min, unsigned int* max) const;
	void getGainRange(unsigned int* min, unsigned int* max) const;
	void getExposureRange(unsigned int* min, unsigned int* max) const;
	void getShutterRange(unsigned int* min, unsigned int* max) const;
	void getFeatureRange(dc1394feature_t feature, unsigned int* min, unsigned int* max) const;
	
	unsigned int getBrightness();
	unsigned int getGamma();
	unsigned int getGain();
	unsigned int getExposure();
	unsigned int getShutter();
	unsigned int getFeature(dc1394feature_t feature);
	
	float getBrightnessNorm();
	float getGammaNorm();
	float getGainNorm();
	float getExposureNorm();
	float getShutterNorm();
	float getFeatureNorm(dc1394feature_t feature);
	
	float getShutterAbs() const;
	float getFeatureAbs(dc1394feature_t feature) const;
	
	void grabStill(ofImage& img);
	bool grabVideo(ofImage& img, bool dropFrames = true);
	
	void flushBuffer();
		
	unsigned int getWidth() const;
	unsigned int getHeight() const;
	int getImageType() const;
	void printFeatures() const;
	
	dc1394camera_t* getLibdcCamera();

protected:
	static dc1394_t* libdcContext;
	static int libdcCameras;
	static void startLibdcContext();
	static void stopLibdcContext();
	
	static int getOfImageType(dc1394color_coding_t libdcType);
	static dc1394color_coding_t getLibdcType(int ofImageType);
		
	dc1394camera_t* camera;
	dc1394video_mode_t videoMode;
	dc1394capture_policy_t capturePolicy;
	unsigned int width, height, left, top;
	int imageType;
	bool useFormat7;
	int format7Mode;
	bool use1394b;
	
	bool grabFrame(ofImage& img);
	void initCamera(int cameraNumber);
	void applySettings();
	
	void quantizeSize();
	void quantizePosition();
	
	void setTransmit(bool transmit);
};