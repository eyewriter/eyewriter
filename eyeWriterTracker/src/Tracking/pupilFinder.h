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
	bool update(ofxCvGrayscaleAdvanced & _currentImg, int threshold, int minBlobSize, int maxBlobSize);

	ofxCvGrayscaleAdvanced	currentImg;
	ofxCvGrayscaleAdvanced	previousImg;
	ofxCvGrayscaleAdvanced	diffImg;

	ofxCvGrayscaleImage		imgBeforeThreshold;			// only for displaying in tracking mode
	
	ofxCvContourFinder		contourFinder;
	ofxCvBlob				foundPupilBlob;

	bool					firstFrame;

	ellipseCalcurator		ellipseCal;
		
	float		convMin;					// convertTorange		default min:0 , max 255	... needed?
	float		convMax;					// change the brightness range into convMin - convMax

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
