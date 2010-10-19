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
#include "homographyCalcurator.h"
#include "brightDarkFinder.h"
#include "thresholdCalculator.h"

typedef struct {
	bool bEyeFound;
	bool bGoodAlternation;
	bool bGlintInBrightEyeFound;
	bool bGlintInDarkEyeFound;
	bool bPupilFound;
} trackingState;


class eyeTracker {
	
public:
	
	eyeTracker();
	void		setup(int width, int height);
	void		update(ofxCvGrayscaleImage & grayImgFromCam);
	ofPoint		getGlintPoint(int glintID);
	ofPoint		getVectorGlintToPupil(int glintID);			// final eye tracked position. (not gaze estimation)
	
	ofxCvGrayscaleAdvanced			currentImg;
	ofxCvGrayscaleImage				brightEyeImg;
	ofxCvGrayscaleImage				darkEyeImg;
	ofxCvGrayscaleAdvanced			magCurrentImg;
	ofxCvGrayscaleAdvanced			smallCurrentImg;
	
	eyeFinder						eFinder;
	pupilFinder						pFinder;
	glintFinder						gFinder;
	
	thresholdCalculator				thresCal;
	
	ofPoint			pupilCentroid;
	ofRectangle		targetRect;
	
	ofRectangle		targetRectDark;		// temporary for draw diagnostic on input image.
	ofRectangle		targetRectBright;	// temporary for draw diagnostic on input image.

	float			magRatio;
	int				w, h;
	bool			bFoundEye;

	// should keep thresholds here for auto threshold stuff.
	// but.. make struct.. or think about it. to make more readable codes.
	// 
	
	trackingState	tState;
	
	//eye Tracking
	float			divisor;
	float			threshold_e;
	float			minBlobSize_e;
	float			maxBlobSize_e;
	
	//pupil Tracking
	bool			bUseAutoThreshold_p;
	float			threshold_p_frompanel;
	float			threshold_p; 
	float			minBlobSize_p;
	float			maxBlobSize_p;
	
	//glint Tracking
	bool			bUseAutoThreshold_g;
	float			threshold_g_frompanel;
	float			threshold_g;
	float			minBlobSize_g;
	float			maxBlobSize_g;
	
	//Bright/Dark Threshold
	bool				bFoundOne;
	brightDarkFinder	briDarkFinder;
	
	ofPoint				currentEyePoint;
	bool				bIsBrightEye;
	
	bool				bUseGlintinBrightEye;
	
	// Warp, Homography
	bool					bUseHomography;	
	homographyCalcurator	homographyCal;
	
	
protected:
	
	bool	getBrightEyeDarkEye();
	
};

#endif //_EYE_TRACKER_

