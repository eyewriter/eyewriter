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
	divisor =2;					// for eyeFinder, make the picture smaller to reduce computation
	
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
	
	brightEyeImg.allocate(targetWidth, targetHeight);
	darkEyeImg.allocate(targetWidth, targetHeight);
	
	smallTargetWidth = targetWidth / divisor;
	smallTargetHeight = targetHeight / divisor;
	
	smallCurrentImg.allocate(smallTargetWidth, smallTargetHeight);
	notDiffImg.allocate(smallTargetWidth, smallTargetHeight);
	
	eFinder.setup(w, h, targetWidth, targetHeight, divisor);
	pFinder.setup(targetWidth, targetHeight, magRatio, divisor);
	gFinder.setup(targetWidth, targetHeight, magRatio);
	
	homographyCal.setup(100, 50, targetWidth * magRatio, targetHeight * magRatio);
	
}

//----------------------------------------------------
void eyeTracker::update(ofxCvGrayscaleImage & grayImgFromCam){
	
	currentImg.setFromPixels(grayImgFromCam.getPixels(), grayImgFromCam.width, grayImgFromCam.height);
	
	// get the eye position
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
			
			// get the averages of pupil & white part.
			if (bUseAutoThreshold_g || bUseAutoThreshold_p){
				getAverages();
			}
			
			if (bUseAutoThreshold_g) threshold_g = (maxBrightness + whiteAvg)/2;
			else threshold_g = threshold_g_frompanel;
			
			// get glint position with dark eye image.
			gFinder.update(magCurrent, threshold_g, minBlobSize_g, maxBlobSize_g);
			
			bool bFoundPupil;
			
			if (gFinder.bFound){
				
				if (bUseAutoThreshold_p) threshold_p = (pupilAvg + whiteAvg) / 2;
				else threshold_p = threshold_p_frompanel;
				
				if (bUseHomography && gFinder.bFourGlints){
					// Homography..
					ofxCvGrayscaleAdvanced* temp = homographyCal.getWarpedImage(magCurrent, gFinder, magRatio);
					bFoundPupil = pFinder.update(*temp, threshold_p, minBlobSize_p, maxBlobSize_p);
					
				} else {
					
					bFoundPupil = pFinder.update(magCurrent, threshold_p, minBlobSize_p, maxBlobSize_p);
				}
				
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
	float	tempAvg = 	cvAvg(currentImg.getCvImage()).val[0];

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
		ofPoint* temp;
		temp = homographyCal.getDstPos();
		return (temp[glintID] / magRatio) + ofPoint(targetRect.x, targetRect.y);
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
void eyeTracker::getAverages(){
	
	smallCurrentImg.scaleIntoMe(magCurrent, CV_INTER_NN);			// which one is the fastest? NN? / LINEAR?

	eFinder.diffImg.setROI(targetRect.x/divisor, targetRect.y/divisor, smallTargetWidth, smallTargetHeight);
	CvScalar tempPupilAvg = cvAvg(smallCurrentImg.getCvImage(), eFinder.diffImg.getCvImage());
	cvNot(eFinder.diffImg.getCvImage(), notDiffImg.getCvImage());
	CvScalar tempWhiteAvg = cvAvg(smallCurrentImg.getCvImage(), notDiffImg.getCvImage());
	eFinder.diffImg.resetROI();
	
	currentImg.setROI(targetRect);
	double tempMin, tempMax;
	cvMinMaxLoc(currentImg.getCvImage(), &tempMin, &tempMax);
	currentImg.resetROI();
	
	maxBrightness = (float)tempMax;
	pupilAvg = tempPupilAvg.val[0];
	whiteAvg = tempWhiteAvg.val[0];
	
}
//----------------------------------------------------



