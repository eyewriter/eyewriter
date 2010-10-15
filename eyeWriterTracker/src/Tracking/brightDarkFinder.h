#pragma once

#include "ofMain.h"
#include "ofxOpenCv.h"
#include "ofxCvGrayscaleAdvanced.h"

class brightDarkFinder {
	
	public:
		
		bool getBrightEyeDarkEye(ofxCvGrayscaleAdvanced & img, ofRectangle & roi);
		void drawAutoThresholdBrightnessGraph(int x, int y);
	
		vector	<float>		averageVec;
		float				pixelAvg;
		float				pixelAvginTenframes;

};