#ifndef _EYE_FINDER_
#define _EYE_FINDER_

#include "ofMain.h"
#include "ofxCvGrayscaleAdvanced.h"
#include "ofxOpenCv.h"
#include "ofxVectorMath.h"

class eyeFinder  {
	
public:
	
	void setup(int width, int height, int _targetWidth, int _targetHeight, int divisor = 1);
	bool update(ofxCvGrayscaleAdvanced & _currentImg, float threshold, float minBlobSize, float maxBlobSize, bool bAllArea);
	void draw(float x, float y, float width, float height, bool bError);
	void getRotatedBoundingBox(int blobNum);
	
	ofxCvContourFinder			contourFinder;
	ofxCvGrayscaleAdvanced		currentImg;
	ofxCvGrayscaleAdvanced		previousImg;
	ofxCvGrayscaleAdvanced		diffImg;

	//!!!: only to check
	//	ofxCvGrayscaleAdvanced		tempCurrent;
	//	ofxCvGrayscaleAdvanced		tempPrevious;
	//	ofxCvGrayscaleAdvanced		temptest_dif;	
	
	ofxCvBlob						foundBlob;
	
	float			w, h, div;
	float			wdiv, hdiv;
	int				targetWidth, targetHeight;
	
	ofRectangle		offsetROI;
	ofRectangle		boundingRect;
	ofPoint			centroid;
	
	bool			bBadFrame;
	bool			bFoundOne;
	bool			bFirstFrame;
	
	// Filter
	
	bool			bUseGamma;
	float			gamma;
	bool			bUseDilate;
	int				nErosions, nDilations;
	bool			bUseBlur;
	float			blur;
	bool			bUseContrast;
	float			contrast;
	float			brightness;
	
	// Shape Filter
	
	bool			bUseCompactnessTest;
	float			maxCompactness;
	bool			bUseLongShortTest;
	float			minLongShortRatio;
	CvBox2D			box;
	
	float			maxWidth;
	float			maxHeight;
	float			minSquareness;
	
	
private:
	
	ofRectangle	findingRect;
	
};

#endif /* _EYE_FINDER_ */


