#include "eyeTracker.h"
//--------------------------------------------------------------
eyeTracker::eyeTracker() {
	
	bFoundOne	= false;
}

//----------------------------------------------------
void eyeTracker::setup(int width, int height) {
	
	w = width;					// IM.width
	h = height;					// IM.height
	magRatio = 4;				// for accurate detection. width x magRatio, height x magRatio
	divisor = 2;				// for eyeFinder, make the picture smaller to reduce computation
	
	// TODO: now it can't work with divisor = 1, let's debug this problem.
	
	printf("VideoWidth, VideoHeight = %d, %d \n", w, h);
	
	currentImg.allocate(w, h);
	currentEyePoint	= ofPoint(0, 0);
	
	targetRect.x = 0;
	targetRect.y = 0;
	targetRect.width = 128;		// for zach 128, 88 was default, might ought to be devided by 4 or 2(for resize)?
	targetRect.height = 64;
	
	magCurrentImg.allocate(targetRect.width * magRatio, targetRect.height * magRatio);
	smallCurrentImg.allocate(w/divisor, h/divisor);
	
	brightEyeImg.allocate(targetRect.width, targetRect.height);
	darkEyeImg.allocate(targetRect.width, targetRect.height);
		
	eFinder.setup(w, h, targetRect.width, targetRect.height, divisor);
	pFinder.setup(targetRect.width, targetRect.height, magRatio, w, h);
	gFinder.setup(targetRect.width, targetRect.height, magRatio, w, h);
	
	thresCal.setup(targetRect.width, targetRect.height, divisor);
	homographyCal.setup(100, 50, targetRect.width * magRatio, targetRect.height * magRatio);
	
	bUseGlintinBrightEye = false;	
	bFoundEye = false;
	
	// initialize status.
	tState.bEyeFound = false;
	tState.bGoodAlternation = false;
	tState.bGlintInBrightEyeFound = false;
	tState.bGlintInDarkEyeFound = false;
	tState.bPupilFound = false;
	
}

//----------------------------------------------------
void eyeTracker::update(ofxCvGrayscaleImage & grayImgFromCam) {
	
	// initialize status, keep status for pupil & glint.
	tState.bEyeFound = false;
	tState.bGoodAlternation = false;
	
	// get the image from input manager.
	currentImg.setFromPixels(grayImgFromCam.getPixels(), grayImgFromCam.width, grayImgFromCam.height);	

	// get the small size image to find eye position.
	if (divisor !=1) smallCurrentImg.scaleIntoMe(currentImg, CV_INTER_LINEAR);
	
	// get the eye position
	bFoundOne = eFinder.update(smallCurrentImg, threshold_e, minBlobSize_e, maxBlobSize_e, true);	
	
	if (eFinder.centroid.x > w - (targetRect.width/2) || eFinder.centroid.x < (targetRect.width/2) ||
		eFinder.centroid.y > h - (targetRect.height/2) || eFinder.centroid.y < (targetRect.height/2)){
		bFoundOne = false;
	}

	bFoundEye = false;
	bool bFoundPupil = false;
	
	tState.bEyeFound = bFoundOne;				// update the trackingState.
	
	if (bFoundOne){
		
		targetRect.x = eFinder.centroid.x - (targetRect.width/2);
		targetRect.y = eFinder.centroid.y - (targetRect.height/2);
		
		// make big eye image
		currentImg.setROI(targetRect);
		
		if (magRatio != 1) {
			magCurrentImg.scaleIntoMe(currentImg, CV_INTER_CUBIC);			// magnify by bicubic
		} else {
			magCurrentImg.setFromPixels(currentImg.getRoiPixels(), targetRect.width, targetRect.height);
		}
		
		currentImg.resetROI();
		
		// get current bright eye image & dark eye image, and compare current status with past status.
		bool	bWasBrightEye = bIsBrightEye;
		bIsBrightEye = getBrightEyeDarkEye();		
		if (bWasBrightEye != bIsBrightEye) tState.bGoodAlternation = true;
		
		// get glint position in a bright eye image, if needed. <= should be here..? think about it.
		if (bIsBrightEye && bUseGlintinBrightEye) {
			thresCal.update(smallCurrentImg, eFinder.diffImg, currentImg, targetRect, true);			// update threshold for glint in bright eye.
			tState.bGlintInBrightEyeFound = gFinder.findGlintCandidates(magCurrentImg, thresCal.getGlintThreshold(true), minBlobSize_g, maxBlobSize_g, true);
			targetRectBright = targetRect;
		}
		
		// find Pupil image again with the big eye image. (try double ellipse fit later?)
		if (!bIsBrightEye){
			
			// get the averages of pupil & white part.
			if (bUseAutoThreshold_g || bUseAutoThreshold_p){
				thresCal.update(smallCurrentImg, eFinder.diffImg, currentImg, targetRect, false);
			}
			
			if (bUseAutoThreshold_g) threshold_g = thresCal.getGlintThreshold(false);
			else threshold_g = threshold_g_frompanel;
			
			// get glint position with dark eye image.
			gFinder.update(magCurrentImg, threshold_g, minBlobSize_g, maxBlobSize_g, bUseGlintinBrightEye);
			tState.bGlintInDarkEyeFound = gFinder.bFound;
			
			if (gFinder.bFound){
								
				if (bUseAutoThreshold_p) threshold_p = thresCal.getPupilThreshold();
				else threshold_p = threshold_p_frompanel;
				
				if (bUseHomography && gFinder.bFourGlints){
					// Homography..
					ofxCvGrayscaleAdvanced* temp = homographyCal.getWarpedImage(magCurrentImg, gFinder, magRatio);
					bFoundPupil = pFinder.update(*temp, threshold_p, minBlobSize_p, maxBlobSize_p, targetRect);
					
				} else {
					
					bFoundPupil = pFinder.update(magCurrentImg, threshold_p, minBlobSize_p, maxBlobSize_p, targetRect);
				}
				
				tState.bPupilFound = bFoundPupil;
				
				if (bFoundPupil){
					pupilCentroid = pFinder.currentPupilCenter;
					bFoundEye = true;
					targetRectDark = targetRect;
				}
			}
		}
	}	
	//	cout << "bFoundOne: " << bFoundOne << "  bFoundPupil: " << bFoundPupil << " bFoundGlint: " << gFinder.bFound << endl;
}

//----------------------------------------------------
bool eyeTracker::getBrightEyeDarkEye() {
	
	bool bBright = briDarkFinder.getBrightEyeDarkEye(currentImg, eFinder.boundingRect);

	currentImg.setROI(targetRect);

	if (bBright) brightEyeImg.setFromPixels(currentImg.getRoiPixels(), targetRect.width, targetRect.height);
	else darkEyeImg.setFromPixels(currentImg.getRoiPixels(), targetRect.width, targetRect.height);
	
	currentImg.resetROI();
	return bBright;
}

//----------------------------------------------------
ofPoint	eyeTracker::getGlintPoint(int glintID) {
	
	if (bUseHomography && gFinder.bFourGlints){
		ofPoint* temp;
		temp = homographyCal.getDstPos();
		return (temp[glintID] / magRatio);
	} else {
		return gFinder.getGlintPosition(glintID);
	}
}

//----------------------------------------------------
ofPoint	eyeTracker::getVectorGlintToPupil(int glintID) {
	
	if (pupilCentroid.x != 0 && gFinder.bFound == true){
		return (pupilCentroid - getGlintPoint(glintID));
	}
	return ofPoint(0,0);
}

//----------------------------------------------------




// a bug is in the "findContours" in ofxCvContourFinder.
// I changed the order of lines.
// see findContour in ofxCvContourFinder.		
//	
// To use findContour with ROI, I changed the codes like below.
//
// From:
// inputCopy = input;
// inputCopy.setROI( input.getROI() );
//
// To:
// inputCopy.setROI( input.getROI() );
// inputCopy = input;


