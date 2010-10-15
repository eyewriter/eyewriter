#pragma once

#include "ofMain.h"
#include "ofxOpenCv.h"

enum glintElementNum {
	GLINT_ELEMENT_LEFT = 0,
	GLINT_ELEMENT_RIGHT = 1,
};

enum glintCombinationNum {
	GLINT_ELEMENT_BOTTOM = 0,
	GLINT_ELEMENT_TOP = 1,
};


typedef struct { 
	int area;
	ofRectangle	boundingRect;
} glintUnit;

typedef struct {
	vector < glintUnit > glints[2];
	float avgArea[2];
	float avgWidth[2];
	float avgHeight[2];
	
	float avgDistance;
	
} glintHistory;

class glintRefChecker {
	
public:
	void	setup();
	void	addGlints(ofxCvBlob & refBlobLeft, ofxCvBlob & refBlobRight, bool bIsTopGlints);
	void	calculateAverages();
	bool	checkSize(ofxCvBlob & blob, int glintID, float minPct, float maxPct);
	float	getAverageDistance(bool bIsTopGlints);
	int		getnFrames(bool bIsTopGlints);
	void	clear();
	
	glintHistory	glintHis[2];
	bool			bNeedCalculate;
	
};

