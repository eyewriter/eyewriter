#pragma once

#include "ofMain.h"
#include "ofxOpenCv.h"

enum glintNum {
	
	GLINT_BOTTOM_LEFT = 0,
	GLINT_BOTTOM_RIGHT = 1,
	GLINT_TOP_LEFT = 2,
	GLINT_TOP_RIGHT= 3,
	GLINT_IN_BRIGHT_EYE = 4
};

typedef struct {
	int startx;
	int starty;
	int endx;
	int endy;
	int distance;
	
} lineSegment; 


class glintLineChecker {

public:
	void setup(int eyeImgWidth, int eyeImgHeight);
	void update(ofxCvGrayscaleImage & eyeImg, int nGlints, ofxCvContourFinder & contourFinder, bool bUseGlintInBrightEye, ofxCvContourFinder & contourFinderBright);
	void checkGlintinBrightEye();
	void draw(int x, int y);
	
	vector < lineSegment >	lineSegments;
	ofxCvGrayscaleImage		myStripesImage;
	ofxCvContourFinder		linesFinder;
	int	width, height;
	float minDistance, maxDistance;
	int leftGlintID, rightGlintID;
	bool bDeleteLine;
	
	bool lineInRange (const lineSegment& seg);
	bool lineCrossGlintInBrightEye(const lineSegment& seg, ofxCvContourFinder & contourFinderBright);

};


