#pragma once

#include "ofMain.h"
#include "ofxXmlSettings.h"
#include "ofxControlPanel.h"
#include "ofxLeastSquares.h"

class calibrationFileSaver {
	
public:
	
	void saveCalibration(ofxLeastSquares& ls, int nRefPoints, vector<ofPoint>& referencePoints, vector<ofPoint>& pointsFromls, vector<ofPoint>& eyePoints, vector<ofPoint>& screenPoints);
	void loadCalibration(ofxLeastSquares& ls, vector<ofPoint>& referencePoints, vector<ofPoint>& pointsFromls, vector<ofPoint>& eyePoints, vector<ofPoint>& screenPoints);
	
};

