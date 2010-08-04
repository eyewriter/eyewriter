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
	void	getAverages();								//get the brightness averages of pupil & white part
	
	ofxCvGrayscaleAdvanced			currentImg;
	
	ofxCvGrayscaleImage				brightEyeImg;
	ofxCvGrayscaleImage				darkEyeImg;
	
	ofxCvGrayscaleAdvanced			magCurrent;
	
	ofxCvGrayscaleImage				notDiffImg;
	ofxCvGrayscaleImage				smallCurrentImg;
	
	eyeFinder						eFinder;
	pupilFinder						pFinder;
	glintFinder						gFinder;
	
	float			smallTargetWidth;
	float			smallTargetHeight;
	
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
	
	float			pupilAvg;
	float			whiteAvg;
	float			maxBrightness;
	
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
	homographyCalcurator	homographyCal;
	

	
	
protected:
	
	bool	getBrightEyeDarkEye();
	
	
};

#endif //_EYE_TRACKER_

