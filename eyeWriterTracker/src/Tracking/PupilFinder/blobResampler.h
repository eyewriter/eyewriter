#pragma once

#include "ofMain.h"
#include "ofxOpenCv.h"

class blobResampler {
	
public:
	
	void resample(ofxCvBlob & blob, float pickupRatio, int minimumPts);
	float getLength(ofPoint point);
	float getWholeLength(vector <ofPoint> & pts);
	
};