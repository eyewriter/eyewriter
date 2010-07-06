/*
 *  eyeTracker.h
 *  EyeTrackDTTest
 *
 */

#ifndef _EYE_TRACKER_
#define _EYE_TRACKER_

#include "ofMain.h"
#include "ofxCvGrayscaleAdvanced.h"
#include "ofxOpenCv.h"
#include "ofxVectorMath.h"
#include "eyeFinder.h"
#include "pupilFinder.h"
#include "glintFinder.h"
#include "EllipseCalcurator.h"

class eyeTracker {
	
public:
	
	eyeTracker();
	void		setup(int width, int height);
	void		update(ofxCvGrayscaleImage & grayImgFromCam);
	
	void		drawGlintLine(float x, float y, float width, float height);
	void		drawPupilLine(float x, float y, float width, float height);
	void		drawEllipse(float x, float y, float width, float height);
	
	ofPoint	getEyePoint();
	ofPoint	getGlintPoint(int glintID);
	ofPoint	getVectorGlintToPupil(int glintID);		
	
	ofxCvGrayscaleAdvanced			currentImg;
	
	ofxCvGrayscaleImage				brightEyeImg;
	ofxCvGrayscaleImage				darkEyeImg;
	
	ofxCvGrayscaleAdvanced			magCurrent;
	ofxCvGrayscaleAdvanced			warpedImg;

	eyeFinder						eFinder;
	pupilFinder						pFinder;
	glintFinder						gFinder;
	
	ofPoint		pupilCentroid;
	ofRectangle	targetRect;
	
	int				targetWidth;
	int				targetHeight;
	float			magRatio;
	float			divisorEyeFinder;
	
	int				w, h;
	int				bigEyeSizeWidth;
	int				bigEyeSizeHeight;
	bool			bFirstFrame;
	
	//eye Tracking
	float			threshold_e;
	float			minBlobSize_e;
	float			maxBlobSize_e;
	
	//pupil Tracking
	float			threshold_p;
	float			minBlobSize_p;
	float			maxBlobSize_p;
	
	//glint Tracking
	float			threshold_g;
	float			minBlobSize_g;
	float			maxBlobSize_g;
	
	//Bright/Dark Threshold
	float			threshold_bd;
	
	bool			bFoundOne;
	bool			firstFrame;
	
	float			threshold_temp;
	
	ofPoint			currentEyePoint;
	
	float			pixelAvg;
	float			pixelAvginTenframes;
	vector	 <float> averageVec;
	
	bool			bIsBrightEye;
	
	// Warp
	bool			bUseHomography;	
	ofPoint			srcPos[4];
	ofPoint			dstPos[4];

	
	
protected:
	
	bool	getBrightEyeDarkEye();
	
	
};

#endif //_EYE_TRACKER_

