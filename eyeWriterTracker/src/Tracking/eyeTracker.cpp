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
	
	magCurrent.allocate(targetWidth * magRatio, targetHeight * magRatio);
	warpedImg.allocate(targetWidth * magRatio, targetHeight * magRatio);
	
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
	
	bFoundOne = eFinder.update(currentImg, threshold_e, minBlobSize_e, maxBlobSize_e, true);	
	
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
			magCurrent.setFromPixels(currentImg.getRoiPixels(), targetRect.width, targetRect.height);
		}
		
		currentImg.resetROI();
		
		// get current Bright Eye Image & Dark Eye Image
		bIsBrightEye = getBrightEyeDarkEye();
		
		// find Pupil image again with the big eye image. (try double ellipse fit later?)
		if (!bIsBrightEye){
			
			// get glint position with dark eye image.
			gFinder.update(magCurrent, threshold_g, minBlobSize_g, maxBlobSize_g);
			
			bool bFoundPupil;
			
			if (gFinder.bFound){
				if (bUseHomography && gFinder.bFourGlints){
					// Warp!!!
										
					ofPoint averageSrcPos;
					
					float	hWidth = 100;
					float	hHeight = 50;
					
					srcPos[0] = gFinder.getGlintPosition(GLINT_TOP_LEFT) * magRatio;
					srcPos[1] = gFinder.getGlintPosition(GLINT_TOP_RIGHT) * magRatio;
					srcPos[2] = gFinder.getGlintPosition(GLINT_BOTTOM_LEFT) * magRatio;
					srcPos[3] = gFinder.getGlintPosition(GLINT_BOTTOM_RIGHT) * magRatio;
					
					averageSrcPos = (srcPos[0] + srcPos[1] + srcPos[2] + srcPos[3]) / 4;

					// Destination positions(shape) should be calibrated when we calibrate..?
					// like, just see one point on the screen, and get the shape of the glints. 
					
					dstPos[0] = ofPoint(averageSrcPos.x - hWidth / 2, averageSrcPos.y - hHeight / 2);
					dstPos[1] = ofPoint(averageSrcPos.x + hWidth / 2, averageSrcPos.y - hHeight / 2);
					dstPos[2] = ofPoint(averageSrcPos.x - hWidth / 2, averageSrcPos.y + hHeight / 2);
					dstPos[3] = ofPoint(averageSrcPos.x + hWidth / 2, averageSrcPos.y + hHeight / 2);
										
					warpedImg.warpIntoMe(magCurrent, srcPos, dstPos);
					
					bFoundPupil = pFinder.update(warpedImg, threshold_p, minBlobSize_p, maxBlobSize_p);
					
				} else {
					
					bFoundPupil = pFinder.update(magCurrent, threshold_p, minBlobSize_p, maxBlobSize_p);

				}
				
				//		bool bFoundPupil = pFinder.update(magCurrent, pixelAvginTenframes - threshold_p, minBlobSize_p, maxBlobSize_p);
				
				if (bFoundPupil){
					pupilCentroid = pFinder.currentPupilCenter;
					
				}
			}
		}
	}	
}

//----------------------------------------------------
bool	eyeTracker::getBrightEyeDarkEye(){
	
	currentImg.setROI(eFinder.boundingRect);
	
	IplImage* tempImage = currentImg.getCvImage();
	float	tempAvg = 	cvAvg(tempImage).val[0];
	
	averageVec.push_back(tempAvg);
	
	if (averageVec.size() > 320) averageVec.erase(averageVec.begin()); // calculation needs only 10, use 320 just for displaying.
	
	pixelAvginTenframes = 0;
	
	if (averageVec.size() > 10){
		for (int i = 0; i < 10; i++) {
			pixelAvginTenframes += averageVec[averageVec.size()-i-1];
		}
		pixelAvginTenframes /= 10;
	}
		
	currentImg.resetROI();	
	currentImg.setROI(targetRect);
	
	if (pixelAvginTenframes < tempAvg){
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
ofPoint	eyeTracker::getGlintPoint(int glintID){
	
	if (bUseHomography && gFinder.bFourGlints){
		return (dstPos[glintID] / magRatio) + ofPoint(targetRect.x, targetRect.y);
	} else {
		return gFinder.getGlintPosition(glintID) + ofPoint(targetRect.x, targetRect.y);
	}
	
}

//----------------------------------------------------
ofPoint	eyeTracker::getVectorGlintToPupil(int glintID){
	
	if (pupilCentroid.x != 0 && gFinder.bFound == true){
		return (getEyePoint() - getGlintPoint(glintID));
	}
	return ofPoint(0,0);
}

//----------------------------------------------------------------------------
void eyeTracker::drawGlintLine(float x, float y, float width, float height){
	
	ofSetColor(0, 0, 200, 100);
	ofLine(x + (getGlintPoint(GLINT_BOTTOM_LEFT).x * width/w), y, x + (getGlintPoint(GLINT_BOTTOM_LEFT).x * width/w), y+height);
	ofLine(x, y + (getGlintPoint(GLINT_BOTTOM_LEFT).y * height/h), x + width, y + (getGlintPoint(GLINT_BOTTOM_LEFT).y * height/h));
	
	ofSetColor(0, 200, 0, 100);
	ofLine(x + getGlintPoint(GLINT_BOTTOM_RIGHT).x * width/w, y, x + getGlintPoint(GLINT_BOTTOM_RIGHT).x * width/w, y+height);
	ofLine(x, y + getGlintPoint(GLINT_BOTTOM_RIGHT).y * height/h, x + width, y + (getGlintPoint(GLINT_BOTTOM_RIGHT).y * height/h));
	
	if (gFinder.bFourGlints) {
		
		ofSetColor(0, 200, 200, 100);
		ofLine(x + (getGlintPoint(GLINT_TOP_LEFT).x * width/w), y, x + (getGlintPoint(GLINT_TOP_LEFT).x * width/w), y+height);
		ofLine(x, y + (getGlintPoint(GLINT_TOP_LEFT).y * height/h), x + width, y + (getGlintPoint(GLINT_TOP_LEFT).y * height/h));
		
		ofSetColor(200, 200, 0, 100);
		ofLine(x + getGlintPoint(GLINT_TOP_RIGHT).x * width/w, y, x + getGlintPoint(GLINT_TOP_RIGHT).x * width/w, y+height);
		ofLine(x, y + getGlintPoint(GLINT_TOP_RIGHT).y * height/h, x + width, y + (getGlintPoint(GLINT_TOP_RIGHT).y * height/h));
		
	}
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




