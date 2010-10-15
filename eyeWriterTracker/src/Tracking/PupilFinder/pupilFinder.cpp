#include "pupilFinder.h"

//----------------------------------------------------
void pupilFinder::setup(int width, int height, float _magRatio, float IMwidth, float IMheight){

	magRatio = _magRatio;
	inputWidth = IMwidth;
	inputHeight = IMheight;
	
	currentImg.allocate((int) (width * magRatio), (int) (height * magRatio));
	imgBeforeThreshold.allocate((int) (width * magRatio), (int) (height * magRatio));
	ellipseCal.setup((int) (width * magRatio), (int) (height * magRatio));

	nDilations = 0;
	nErosions = 0;

	bUseContrast		= false;
	contrast			= 0.2;
	brightness			= 0.0;

	bUseBlur			= true;
	blur				= 5;

	bUseGamma = false;
	gamma = 0.57f;

	bUseAdaptiveThreshold = false;
	bGauss = false;
	offset = 20;
	blocksize = 200;

	bUseContrastStretch = false;

}

//----------------------------------------------------
bool pupilFinder::update(ofxCvGrayscaleAdvanced & _currentImg, int threshold, int minBlobSize, int maxBlobSize, ofRectangle & _targetRect){

	targetRect.x = _targetRect.x;
	targetRect.y = _targetRect.y;
	targetRect.width = _targetRect.width;
	targetRect.height = _targetRect.height;
	
	currentImg = _currentImg;

	if (bUseGamma) currentImg.applyMinMaxGamma(gamma);
	if (bUseBlur) currentImg.blur(blur);							//Great!!(also for subtraction!!!)
	if (bUseContrast) currentImg.applyBrightnessContrast(brightness,contrast);
	if (bUseContrastStretch) currentImg.contrastStretch();

	// only for displaying data in tracking mode
	imgBeforeThreshold = currentImg;
	
	// here's always dark eye.
	
	if (bUseAdaptiveThreshold) currentImg.adaptiveThreshold(blocksize, offset, true, bGauss);
	else currentImg.threshold(threshold, true);

	if (bUseDilate){

		int maxOperations	= MAX(nDilations, nErosions);

		for (int i = 0; i < maxOperations; i++) {
			if (i < nErosions)	currentImg.erode();
			if (i < nDilations)	currentImg.dilate();
		}
	}

	// contourFinder & some check

	int num = contourFinder.findContours(currentImg, minBlobSize, maxBlobSize, 1, true, true);
	
	if (num > 0) {
		resampler.resample(contourFinder.blobs[0], 0.1, 10);

		foundPupilBlob = contourFinder.blobs[0];	//TODO:	think about the fitting the blob to input width & height(before magnified).

		if(ellipseCal.update(foundPupilBlob)){
			currentPupilCenter = ellipseCal.currentEyePoint / magRatio;
			return true;
		} else {
			return false;
		}
	}
}

//----------------------------------------------------
void pupilFinder::draw(float x, float y){

	ofPushMatrix();
	ofTranslate(x, y, 0);
	
	ofEnableAlphaBlending();
	
	ofSetColor(255,255,255, 80);
	currentImg.draw(0, 0);
	foundPupilBlob.draw(0, 0);
	ellipseCal.draw(0, 0);
	
	ofSetColor(0, 0, 255, 255);
	if (contourFinder.blobs.size() > 0) {
		ofBeginShape();
		for (int i = 0; i < contourFinder.blobs[0].pts.size(); i++) {
			ofVertex(contourFinder.blobs[0].pts[i].x, contourFinder.blobs[0].pts[i].y);
		}
		ofEndShape(true);
	}
	
	ofDisableAlphaBlending();
	
	ofSetColor(255, 255, 255);
	ofDrawBitmapString("pupilFinder/EllipseFit", 1, currentImg.height + 12);
	
	ofPopMatrix();
	
}

//----------------------------------------------------

