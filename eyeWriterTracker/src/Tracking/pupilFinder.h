/*
 *  pupilFinder.h
 *  EyeTrackDTTest
 *
 *  Created by itotaka on 5/19/10.
 *  Copyright 2010 YCAM. All rights reserved.
 *
 */

#ifndef _PUPIL_FINDER_
#define _PUPIL_FINDER_

#include "ofMain.h"
#include "ofxCvGrayscaleAdvanced.h"
#include "ofxOpenCv.h"
#include "ofxVectorMath.h"
#include "EllipseCalcurator.h"

class pupilFinder {
public:
	
	void setup(int width, int height, float magRatio);
	bool update(ofxCvGrayscaleAdvanced & _currentImg, float threshold, float minBlobSize, float maxBlobSize);
	
	ofxCvGrayscaleAdvanced	currentImg;
	ofxCvGrayscaleAdvanced	previousImg;
	ofxCvGrayscaleAdvanced	diffImg;
	
	//!!!:temporary!
	ofxCvGrayscaleImage		imgBeforeThreshold;
	
	ofxCvContourFinder		contourFinder;
	ofxCvBlob				foundPupilBlob;
		
	bool					firstFrame;
	
	ellipseCalcurator		ellipseCal;
	
	float		convMin;
	float		convMax;
	
	float		magRatio;
	
	bool		bUseGamma;
	float		gamma;
	
	bool		bUseDilate;
	int			nErosions;
	int			nDilations;
	
	bool		bUseContrast;
	float		contrast;
	float		brightness;
	
	bool		bUseBlur;
	int			blur;
	
	float		maxWidth;						// need?
	float		maxHeight;

	bool		bUseCompactnessTest;
	float		maxCompactness;
	
	bool		bUseAdaptiveThreshold;
	int			offset;
	int			blocksize;
	int			bGauss;
	
	bool		bUseContrastStretch;
	
	ofPoint	currentPupilCenter;
	
};

#endif /*_PUPIL_FINDER_*/