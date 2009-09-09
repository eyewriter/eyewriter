/*
* ofxCvGrayscaleImage.h
* by stefanix, zach
*
* This is essentially an IPL_DEPTH_8U with one channel.
* Pixel values are typically from 0-255.
*
*/


#ifndef OFX_CV_GRAYSCALE_ADV_IMAGE_H
#define OFX_CV_GRAYSCALE_ADV_IMAGE_H

#include "ofxCvGrayscaleImage.h"

#define DEFAULT_BRIGHTNESS 0.0
#define DEFAULT_CONTRAST 0.2


// note: 
//
// this is (a) a fix of some ROI stuff for swapTemp
// and a missing "drawBlobIntoMe" function
// and applyMinMaxGamma and applyBrightnessContrast()
// 
// I think many of these things will get back into SVN version of ofxOpenCv, but are 
// included here so that this app compiled sans too much work in OF 0.06.
// 

class ofxCvGrayscaleAdvanced : public ofxCvGrayscaleImage {


  public:

    ofxCvGrayscaleAdvanced();
    ~ofxCvGrayscaleAdvanced(){};
	
	void	swapTemp();			// this has a fix for ROI stuff needed for 
	
	void	drawBlobIntoMe( ofxCvBlob & blob, int color );
	void	applyMinMaxGamma(float gamma);
	void	applyBrightnessContrast(float brightness, float contrast);

	
private:
		
    uchar		gammaLut[256];
	CvMat*		gammaLutMatrix;
	
	uchar		briConLut[256];
	CvMat*		briConLutMatrix;

    
};



#endif

