/*
 *  glintFinder.h
 *  EyeTrackDTTest
 *
 *  Created by itotaka on 5/19/10.
 *  Copyright 2010 YCAM. All rights reserved.
 *
 */

#ifndef _GLINT_FINDER_
#define _GLINT_FINDER_

#include "ofMain.h"
#include "ofxCvGrayscaleAdvanced.h"
#include "ofxOpenCv.h"
#include "ofxVectorMath.h"

class glintFinder {

public:

	void		setup(int bigEyeWidth, int bigEyeHeight, float magRatio);
	bool		update(ofxCvGrayscaleAdvanced & blackEyeImg, int threshold, int minBlobSize, int maxBlobSize);
	ofPoint		getGlintPosition(bool bIsLeftOrTop);

	ofxCvContourFinder		contourFinder;
	ofxCvGrayscaleImage		eyeImage;

	float				subtractVal;
	float				multiplyVal;

	float				w,h;
	float				magRatio;

	ofPoint				leftOrTopGlintPos;
	ofPoint				rightOrBottomGlintPos;

	int					biggestID;
	int					smallestID;

	bool				bFound;

	ofRectangle			glintROI;
	bool				bIsVerticalLED;

};

#endif /*_GLINT_FINDER_*/
