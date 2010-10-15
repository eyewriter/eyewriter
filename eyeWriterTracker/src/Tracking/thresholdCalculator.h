#pragma once

#include "ofMain.h"
#include "ofxOpenCv.h"
#include "ofxCvGrayscaleAdvanced.h"

class thresholdCalculator {
	
public:
	
	void	setup(int width, int height, float _divisor);
	void	update(ofxCvGrayscaleAdvanced & smallCurrentImg, ofxCvGrayscaleAdvanced & smallMaskImg, ofxCvGrayscaleAdvanced & currentImg, ofRectangle & targetRect, bool isBrightEye);
	void	calculateAverages(ofxCvGrayscaleAdvanced & smallCurrentImg, ofxCvGrayscaleAdvanced & maskImg, ofRectangle & targetRect);
	void	calculateMinMaxBrightness(ofxCvGrayscaleAdvanced & currentImg, ofRectangle & targetRect);
	float	getPupilThreshold();
	float	getGlintThreshold(bool isBrightEye);
	float	getPupilAvg();
	float	getWhiteAvg();
	float	getMinBrightness();
	float	getMaxBrightness();
	float	getMinInWhite();
	float	getMaxInWhite();
	
	void	drawPupilImageWithScanLine(int x, int y, int w, int h, ofxCvGrayscaleImage & img);
	void	drawBrightnessScanGraph(int x, int y, ofxCvGrayscaleImage & img, bool bIsVertical, float threshold_p, float threshold_g, string graphname);

	
	ofxCvGrayscaleImage		notDiffImg;

	IplImage*		farFromAvg;
	IplImage*		newMask;
	
	float			pupilThreshold;
	float			glintThresholdForDarkEye;
	float			glintThresholdForBrightEye;
	
	float			pupilAvg, whiteAvg;
	double			minBrightness, maxBrightness;
	double			whiteMin, whiteMax;
	CvPoint			whiteLocMin, whiteLocMax;

	
	ofRectangle		roi;
	float			divisor;
	
	int				scanX;
	int				scanY;

};
