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
	//	bIsVerticalLED = false;
	bFourGlints = false;					// 2 or 4
		
	subtractVal = 0.0f;
	multiplyVal = 1.0f;
	
	limitratio = 0.7f;
	
	glintROI.x = 0;
	glintROI.y = 0;
	glintROI.width = w * magRatio;
	glintROI.height = h * magRatio;
	
}

//--------------------------------------------------------------------
bool glintFinder::update(ofxCvGrayscaleAdvanced & blackEyeImg, float threshold, float minBlobSize, float maxBlobSize){
	
	bFound = false;
	
	if (bFourGlints){
		glintROI.x = 0;
		glintROI.y = 0;
		glintROI.width = w * magRatio;
		glintROI.height = h * magRatio;
	} else {
		glintROI.x = 0;
		glintROI.y = h * magRatio/2 - 30;
		glintROI.width = w * magRatio;
		glintROI.height = h * magRatio/2;
	}
	
	eyeImage = blackEyeImg;
	
	eyeImage.setROI(glintROI);
	
	eyeImage.contrastStretch();
	eyeImage.threshold(threshold, false);
	
	int nGlints;
	
	if (bFourGlints) nGlints = 4;
	else nGlints = 2;
	
	int nPossibleGlint = contourFinder.findContours(eyeImage, minBlobSize, maxBlobSize, nGlints + 4, true, true);
	
	eyeImage.resetROI();
	
	
	// edge check.
		
	for (int i = 0; i < contourFinder.blobs.size(); i++) {
		
		if (contourFinder.blobs[i].boundingRect.x < 3 || contourFinder.blobs[i].boundingRect.y < 3 ||
			contourFinder.blobs[i].boundingRect.x + contourFinder.blobs[i].boundingRect.width > w * magRatio - 3 ||
			contourFinder.blobs[i].boundingRect.y + contourFinder.blobs[i].boundingRect.height > h * magRatio - 3){

			contourFinder.blobs.erase(contourFinder.blobs.begin() + i);
			i--;
			
		}
	}
	
	
	// check width / height ratio.
	
	for (int i = 0; i < contourFinder.blobs.size(); i++){
		if ((contourFinder.blobs[i].boundingRect.width / contourFinder.blobs[i].boundingRect.height) < limitratio ||
			(contourFinder.blobs[i].boundingRect.width / contourFinder.blobs[i].boundingRect.height) > (1/limitratio)){
			
			contourFinder.blobs.erase(contourFinder.blobs.begin() + i);
			i--;
			
		}
	}
	
	// can do more. but for now. check each glint when more glints are found. or make glint tracker?
	// or check x and y coodinates.
	
			
	if (contourFinder.blobs.size() >= nGlints){
				
		ofPoint averagePoint = 0;
				
		for (int i = 0; i < nGlints; i++) {
			averagePoint += contourFinder.blobs[i].centroid;
		}
		
		averagePoint /= nGlints;
		
		if (!bFourGlints){
			for (int i = 0; i < nGlints; i++) {
				ofPoint gp = contourFinder.blobs[i].centroid;
				
				if (gp.x < averagePoint.x) glintPos[GLINT_BOTTOM_LEFT] = gp / magRatio;
				else glintPos[GLINT_BOTTOM_RIGHT] = gp / magRatio;
			}
		} else {
			
			for (int i = 0; i < nGlints; i++) {
				ofPoint gp = contourFinder.blobs[i].centroid;
				
				if (gp.x < averagePoint.x && gp.y < averagePoint.y) glintPos[GLINT_TOP_LEFT] = gp / magRatio;
				else if (gp.x >= averagePoint.x && gp.y < averagePoint.y) glintPos[GLINT_TOP_RIGHT] = gp / magRatio;
				else if (gp.x < averagePoint.x && gp.y >= averagePoint.y) glintPos[GLINT_BOTTOM_LEFT] = gp / magRatio;
				else if (gp.x >= averagePoint.x && gp.y >= averagePoint.y) glintPos[GLINT_BOTTOM_RIGHT] = gp / magRatio;
				
			}
		}
		
		bFound = true;
		
	}
	
	return bFound;
}

//--------------------------------------------------------------------
ofPoint	glintFinder::getGlintPosition(int glintID){

	// this is glint positions in the targetRect.
	
	return ofPoint(glintPos[glintID].x + glintROI.x / magRatio , glintPos[glintID].y + glintROI.y / magRatio);
	
}

//--------------------------------------------------------------------
