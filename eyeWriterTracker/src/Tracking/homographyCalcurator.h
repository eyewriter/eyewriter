#pragma once

#include "ofMain.h"
#include "ofxOpenCv.h"
#include "glintFinder.h"
#include "ofxCvGrayscaleAdvanced.h"

class homographyCalcurator {
	
public:
	
	void	setup(int widthGlintRectangle, int heightGlintRectangle, int widthWarpedImage, int heightWarpedImage);
	void	draw(int x, int y , int w, int h);
	ofxCvGrayscaleAdvanced*	getWarpedImage(ofxCvGrayscaleImage& magCurrent, glintFinder& gFinder, float magRatio);
	ofPoint*	getDstPos();

	
	ofxCvGrayscaleAdvanced		warpedImg;
	
	ofPoint	srcPos[4];
	ofPoint	dstPos[4];
	
	float	hWidth;
	float	hHeight;

};
