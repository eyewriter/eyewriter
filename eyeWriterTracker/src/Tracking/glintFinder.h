#ifndef _GLINT_FINDER_
#define _GLINT_FINDER_

#include "ofMain.h"
#include "ofxCvGrayscaleAdvanced.h"
#include "ofxOpenCv.h"
#include "ofxVectorMath.h"

enum glintNum {
	
	GLINT_BOTTOM_LEFT = 0,
	GLINT_BOTTOM_RIGHT = 1,
	GLINT_TOP_LEFT = 2,
	GLINT_TOP_RIGHT= 3
	
};

class glintFinder {
	
public:
	
	void		setup(int bigEyeWidth, int bigEyeHeight, float magRatio);
	bool		update(ofxCvGrayscaleAdvanced & blackEyeImg, float _threshold, float minBlobSize, float maxBlobSize);
	ofPoint		getGlintPosition(int glintID);
	
	ofxCvContourFinder		contourFinder;
	ofxCvGrayscaleImage		eyeImage;
	
	bool				bFourGlints;					//true: 4 glints, false: 2 glints
	
	float				subtractVal;
	float				multiplyVal;
	
	float				w,h;
	float				magRatio;
	
	ofPoint				glintPos[4];
	
	int					biggestID;
	int					smallestID;
	float				limitratio;
	
	bool				bUseContrastStretch;
	int					threshold;
	
	bool				bFound;
	
	ofRectangle			glintROI;
	ofPoint				pctGlintROIorigin;
	ofPoint				pctGlintROIend;
	
	//	bool				bIsVerticalLED;
	
};

#endif /*_GLINT_FINDER_*/



