/*
 *  pupilFinder.cpp
 *  EyeTrackDTTest
 *
 *  Created by itotaka on 5/19/10.
 *  Copyright 2010 YCAM. All rights reserved.
 *
 */

#include "pupilFinder.h"

//----------------------------------------------------
void pupilFinder::setup(int width, int height, float _magRatio){

	magRatio = _magRatio;

	currentImg.allocate((int) (width * magRatio), (int) (height * magRatio));
	imgBeforeThreshold.allocate((int) (width * magRatio), (int) (height * magRatio));


	ellipseCal.setup((int) (width * magRatio), (int) (height * magRatio));

	nDilations = 0;
	nErosions = 0;

	firstFrame			= true;

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

	convMin = 0;
	convMax = 255;
	bUseContrastStretch = true;

}

//----------------------------------------------------
bool pupilFinder::update(ofxCvGrayscaleAdvanced & _currentImg, int threshold, int minBlobSize, int maxBlobSize){

	currentImg = _currentImg;

	if(bUseGamma){
		currentImg.applyMinMaxGamma(gamma);
	}

	if (bUseBlur) {
		currentImg.blur(blur);							//Great!!(also for subtraction!!!)
	}

	if (bUseContrast){
		currentImg.applyBrightnessContrast(brightness,contrast);
	}

	if (bUseContrastStretch){
		currentImg.contrastStretch();
	} else {
		currentImg.convertToRange(convMin, convMax);
	}

	//!!!:temporary!
	imgBeforeThreshold = currentImg;

	if (bUseAdaptiveThreshold) {
		currentImg.adaptiveThreshold(blocksize, offset, true, bGauss);
	} else {
		currentImg.threshold(threshold, true);
	}


	if (bUseDilate){

		int maxOperations		= MAX(nDilations, nErosions);

		for (int i = 0; i < maxOperations; i++) {

			if (i < nErosions)	currentImg.erode();
			if (i < nDilations)	currentImg.dilate();

		}
	}

	// contourFinder & some check

	int num = contourFinder.findContours(currentImg, minBlobSize, maxBlobSize, 1, true, true);

	if (num > 0) {

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
