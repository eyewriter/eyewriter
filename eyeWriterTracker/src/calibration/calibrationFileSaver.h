#pragma once

#include "ofMain.h"
#include "ofxXmlSettings.h"
#include "ofxControlPanel.h"
#include "ofxLeastSquares.h"

class calibrationFileSaver {
	
public:
	
	void saveCalibration(ofxLeastSquares& ls, int nRefPoints, float spx[], float spy[], ofPoint cp[], vector<ofPoint>& eyePoints, vector<ofPoint>& screenPoints);
	void loadCalibration(ofxLeastSquares& ls, float spx[], float spy[], ofPoint cp[], vector<ofPoint>& eyePoints, vector<ofPoint>& screenPoints);
	
};

