/*
 *  eyeTracker.cpp
 *  EyeTrackDTTest
 *
 */

#include "eyeTracker.h"
//--------------------------------------------------------------
eyeTracker::eyeTracker(){

	bFoundOne	= false;
	firstFrame = true;

}

//----------------------------------------------------
void eyeTracker::setup(int width, int height){

	w = width;					// IM.width
	h = height;					// IM.height
	magRatio = 4;				// for accurate detection. width x magRatio, height x magRatio


	printf("VideoWidth, VideoHeight = %d, %d \n", w, h);

	currentImg.allocate(w, h);

	currentEyePoint	= ofPoint(0,0);

	targetWidth = 88;
	targetHeight = 64;

	targetRect.x = 0;
	targetRect.y = 0;
	targetRect.width = targetWidth;
	targetRect.height = targetHeight;

	magCurrent.allocate((int) (targetWidth * magRatio), (int) (targetHeight * magRatio));

	brightEyeImg.allocate(targetWidth, targetHeight);
	darkEyeImg.allocate(targetWidth, targetHeight);

	// filtering:

	eFinder.setup(w, h, targetWidth, targetHeight, 2);
	pFinder.setup(targetWidth, targetHeight, magRatio);
	gFinder.setup(targetWidth, targetHeight, magRatio);

}

//----------------------------------------------------
void eyeTracker::update(ofxCvGrayscaleImage & grayImgFromCam){

	currentImg.setFromPixels(grayImgFromCam.getPixels(), grayImgFromCam.width, grayImgFromCam.height);

	bFoundOne = eFinder.update(currentImg, (int) threshold_e, (int) minBlobSize_e, (int) maxBlobSize_e, true);

	if (eFinder.centroid.x > w - (targetWidth/2) || eFinder.centroid.x < (targetWidth/2) ||
		eFinder.centroid.y > h - (targetHeight/2) || eFinder.centroid.y < (targetHeight/2)){
		bFoundOne = false;
	}

	if (bFoundOne){

		targetRect.x = eFinder.centroid.x - (targetWidth/2);
		targetRect.y = eFinder.centroid.y - (targetHeight/2);

		// make big EYE image
		currentImg.setROI(targetRect);

		if (magRatio != 1) {
			magCurrent.scaleIntoMe(currentImg, CV_INTER_CUBIC);
		} else {
			magCurrent.setFromPixels(currentImg.getRoiPixels(), (int) targetRect.width, (int) targetRect.height);
		}

		currentImg.resetROI();

		// get current Bright Eye Image & Dark Eye Image
		bIsBrightEye = getBrightEyeDarkEye();

		// find Pupil image again with the big eye image. (try double ellipse fit later?)
		if (!bIsBrightEye){

//			bool bFoundPupil = pFinder.update(magCurrent, pixelAvginTenframes - threshold_p, minBlobSize_p, maxBlobSize_p);
			bool bFoundPupil = pFinder.update(magCurrent, (int) threshold_p, (int) minBlobSize_p, (int) maxBlobSize_p);

			if (bFoundPupil){
				pupilCentroid = pFinder.currentPupilCenter;

				// get glint position with dark eye image.
				gFinder.update(magCurrent, (int) threshold_g, (int) minBlobSize_g, (int) maxBlobSize_g);

				leftOrTopGlintPos = gFinder.getGlintPosition(true);
				rightOrBottomGlintPos = gFinder.getGlintPosition(false);

			}
		}
	}
}

//----------------------------------------------------
bool	eyeTracker::getBrightEyeDarkEye(){

	currentImg.setROI(eFinder.boundingRect);

	/////////// temporary, get pixel average //////////
	// or maybe using just rising or falling is OK?? //
	// all pixels are not needed to make avg.		 //
	//	maybe one-tenth(1/10) are enough?			 //
	///////////////////////////////////////////////////

	unsigned char * tempPixels = currentImg.getRoiPixels();
	pixelAvg = 0;

	for (int i = 0; i < eFinder.boundingRect.width * eFinder.boundingRect.height; i++){
		pixelAvg += tempPixels[i];
	}
	pixelAvg /= eFinder.boundingRect.width * eFinder.boundingRect.height;

	float	tempAvg = pixelAvg;
	averageVec.push_back(tempAvg);

	if (averageVec.size() > 320) averageVec.erase(averageVec.begin());

	pixelAvginTenframes = 0;

	if (averageVec.size() > 10){
		for (int i = 0; i < 10; i++) {
			pixelAvginTenframes += averageVec[averageVec.size()-i-1];
		}
		pixelAvginTenframes /= 10;

	}

	///////////////////////////////////////////////////

	currentImg.resetROI();
	currentImg.setROI(targetRect);

	if (pixelAvginTenframes < pixelAvg){
		brightEyeImg.setFromPixels(currentImg.getRoiPixels(), targetWidth, targetHeight);
		currentImg.resetROI();
		return true;

	} else {
		darkEyeImg.setFromPixels(currentImg.getRoiPixels(), targetWidth, targetHeight);
		currentImg.resetROI();
		return false;
	}
}

//----------------------------------------------------
ofPoint	eyeTracker::getEyePoint(){

	return (pupilCentroid + ofPoint(targetRect.x, targetRect.y));

}

//----------------------------------------------------
ofPoint	eyeTracker::getGlintPoint(bool bIsLeftOrTop){

	if (bIsLeftOrTop)	return leftOrTopGlintPos + ofPoint(targetRect.x, targetRect.y);
	else				return rightOrBottomGlintPos + ofPoint(targetRect.x, targetRect.y);

}

//----------------------------------------------------
ofPoint	eyeTracker::getVectorGlintToPupil(bool bIsLeftOrTopGlint){

	if (pupilCentroid.x != 0 && gFinder.bFound == true){
		return (getEyePoint() - getGlintPoint(bIsLeftOrTopGlint));
	}
	return ofPoint(0,0);
}

//----------------------------------------------------------------------------
void eyeTracker::drawGlintLine(float x, float y, float width, float height){

	ofSetColor(0, 0, 200, 100);
	ofLine(x + (getGlintPoint(true).x * width/w), y, x + (getGlintPoint(true).x * width/w), y+height);
	ofLine(x, y + (getGlintPoint(true).y * height/h), x + width, y + (getGlintPoint(true).y * height/h));

	ofSetColor(0, 200, 0, 100);
	ofLine(x + getGlintPoint(false).x * width/w, y, x + getGlintPoint(false).x * width/w, y+height);
	ofLine(x, y + getGlintPoint(false).y * height/h, x + width, y + (getGlintPoint(false).y * height/h));

}

//----------------------------------------------------
void eyeTracker::drawPupilLine(float x, float y, float width, float height){

	ofSetColor(200, 0, 0, 100);
	ofEnableAlphaBlending();

	ofLine(x + pupilCentroid.x, y,x + pupilCentroid.x , y+height);
	ofLine(x, y + pupilCentroid.y, x + width, y + pupilCentroid.y);

	ofDisableAlphaBlending();

}

//----------------------------------------------------
void eyeTracker::drawEllipse(float x, float y, float width, float height){

	pFinder.ellipseCal.draw(x + targetRect.x* (width/w), y + targetRect.y* (width/w), targetWidth * (width/w), targetHeight * (height/h));

}

//----------------------------------------------------




