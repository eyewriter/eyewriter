/*
 *  glintFinder.cpp
 *  EyeTrackDTTest
 *
 *  Created by itotaka on 5/19/10.
 *  Copyright 2010 YCAM. All rights reserved.
 *
 */

#include "glintFinder.h"
//--------------------------------------------------------------------
void glintFinder::setup(int bigEyeWidth,int bigEyeHeight, float _magRatio){
	
	magRatio = _magRatio;
	
	w = bigEyeWidth;
	h = bigEyeHeight;
	
	eyeImage.allocate(w * magRatio, h * magRatio);
	bFound = false;
	bIsVerticalLED = false;

	
	subtractVal = 0.0f;
	multiplyVal = 1.0f;

	if (bIsVerticalLED) {
	
		glintROI.x = w * magRatio/2 - 30;
		glintROI.y = 0;
		glintROI.width = w * magRatio/2;
		glintROI.height = h * magRatio;
	
	} else {
	
		glintROI.x = 0;
		glintROI.y = h * magRatio/2 - 30;
		glintROI.width = w * magRatio;
		glintROI.height = h * magRatio/2;
		
	}
		
}

//--------------------------------------------------------------------
bool glintFinder::update(ofxCvGrayscaleAdvanced & blackEyeImg, float threshold, float minBlobSize, float maxBlobSize){
	
	eyeImage = blackEyeImg;
	
	eyeImage.setROI(glintROI);
	
	eyeImage.contrastStretch();
	eyeImage.threshold(threshold, false);
	
	int nPossibleGlint = contourFinder.findContours(eyeImage, minBlobSize, maxBlobSize, 2, true, true);
	
	eyeImage.resetROI();
	
	// can do more. but for now.
	float tempBiggestX = 0;
	float tempSmallestX = 1000;
	biggestID = -1;
	smallestID = -1;
	bFound = false;
	
	for (int i = 0; i < nPossibleGlint; i++){
		
		float val;
		if (bIsVerticalLED) val = contourFinder.blobs[i].centroid.y;
		else val = contourFinder.blobs[i].centroid.x;
				
		if (tempBiggestX < val) {
			tempBiggestX = val;
			biggestID = i;
		}
		
		if (tempSmallestX > val) {
			tempSmallestX = val;
			smallestID = i;
		}
	}
		
	if (biggestID != -1 && smallestID != -1){
		
		if (!bIsVerticalLED){
		
			leftOrTopGlintPos = (contourFinder.blobs[smallestID].centroid + ofPoint(0, glintROI.y)) / magRatio;
			rightOrBottomGlintPos = (contourFinder.blobs[biggestID].centroid + ofPoint(0, glintROI.y)) / magRatio;

		} else {
		
			leftOrTopGlintPos = (contourFinder.blobs[smallestID].centroid + ofPoint(glintROI.x, 0)) / magRatio;
			rightOrBottomGlintPos = (contourFinder.blobs[biggestID].centroid + ofPoint(glintROI.x, 0)) / magRatio;
		
		}
		
		
		bFound = true;
	}
	
	return bFound;
	
}

//--------------------------------------------------------------------
ofPoint	glintFinder::getGlintPosition(bool bIsLeftOrTop){

// this is glint positions in the targetRect.
		
	if (bIsLeftOrTop) return leftOrTopGlintPos;
	else return rightOrBottomGlintPos;

}

//--------------------------------------------------------------------
