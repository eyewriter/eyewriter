#ifndef _PUPIL_FINDER_
#define _PUPIL_FINDER_

#include "ofMain.h"
#include "ofxCvGrayscaleAdvanced.h"
#include "ofxOpenCv.h"
#include "ofxVectorMath.h"
#include "EllipseCalcurator.h"
#include "eyeFinder.h"
#include "blobResampler.h"

class pupilFinder {
	
public:

	void setup(int width, int height, float _magRatio, float IMwidth, float IMheight);
	bool update(ofxCvGrayscaleAdvanced & _currentImg, int threshold, int minBlobSize, int maxBlobSize, ofRectangle & _targetRect);
	void draw(float x, float y, bool bError);

	ofxCvGrayscaleAdvanced	currentImg;
	ofxCvGrayscaleImage		imgBeforeThreshold;			// only for displaying in tracking mode
	ofxCvContourFinder		contourFinder;
	ofxCvBlob				foundPupilBlob;
	ellipseCalcurator		ellipseCal;
	
	blobResampler			resampler;
	
	float		magRatio;
	float		inputWidth;
	float		inputHeight;

	// image processing
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
	bool		bUseContrastStretch;

	bool		bUseAdaptiveThreshold;
	int			offset;
	int			blocksize;
	int			bGauss;
	
	// condtion for choosing pupil
	float		maxWidth;						// for blob detection. but .. needed?
	float		maxHeight;
	bool		bUseCompactnessTest;
	float		maxCompactness;

	ofPoint		currentPupilCenter;
	ofRectangle	targetRect;

};

#endif /*_PUPIL_FINDER_*/



