/*
 *  pupilFinder.h
 *  EyeTrackDTTest
 *
 *  Created by itotaka on 5/19/10.
 *
 */

#ifndef _PUPIL_FINDER_
#define _PUPIL_FINDER_

#include "ofMain.h"
#include "ofxCvGrayscaleAdvanced.h"
#include "ofxOpenCv.h"
#include "ofxVectorMath.h"
#include "EllipseCalcurator.h"
#include "eyeFinder.h"

class pupilFinder {
public:

	void setup(int width, int height, float magRatio, float divisor);
	bool update(ofxCvGrayscaleAdvanced & _currentImg, eyeFinder& eFinder, ofRectangle targetRect, int threshold, int minBlobSize, int maxBlobSize);

	ofxCvGrayscaleAdvanced	currentImg;
	ofxCvGrayscaleAdvanced	previousImg;
	ofxCvGrayscaleAdvanced	diffImg;
	ofxCvGrayscaleImage		notDiffImg;
	ofxCvGrayscaleImage		smallCurrentImg;

	ofxCvGrayscaleImage		imgBeforeThreshold;			// only for displaying in tracking mode
	
	ofxCvContourFinder		contourFinder;
	ofxCvBlob				foundPupilBlob;

	bool					firstFrame;

	ellipseCalcurator		ellipseCal;
	
	bool		bUseAutoThreshold;
	float		pupilAvg;
	float		whiteAvg;
	
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

	float		smallTargetWidth;				// targetRect size in eyeFinder
	float		smallTargetHeight;
	
		
	bool		bUseAdaptiveThreshold;
	int			offset;
	int			blocksize;
	int			bGauss;
	int			threshold;

	bool		bUseContrastStretch;

	ofPoint	currentPupilCenter;

};

#endif /*_PUPIL_FINDER_*/
