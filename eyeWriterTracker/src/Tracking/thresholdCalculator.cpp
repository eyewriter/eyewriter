#include "thresholdCalculator.h"

void thresholdCalculator::setup(int width, int height, float _divisor){
	
	divisor = _divisor;
	notDiffImg.allocate(width / divisor, height / divisor);

	roi.x = 0;
	roi.y = 0;
	roi.width = width / divisor;
	roi.height = height / divisor;
	
	farFromAvg = cvCreateImage(cvSize(roi.width, roi.height), IPL_DEPTH_8U, 1);
	newMask = cvCreateImage(cvSize(roi.width, roi.height), IPL_DEPTH_8U, 1);

	scanX = 0;
	scanY = 0;
}

void thresholdCalculator::update(ofxCvGrayscaleAdvanced & smallCurrentImg, ofxCvGrayscaleAdvanced & smallMaskImg, ofxCvGrayscaleAdvanced & currentImg,
								 ofRectangle & targetRect, bool isBrightEye) {

	// glint threshold is still rough..
		calculateAverages(smallCurrentImg, smallMaskImg, targetRect);
		calculateMinMaxBrightness(currentImg, targetRect);

	if (isBrightEye){
		glintThresholdForBrightEye = (maxBrightness * 9 + MAX(whiteAvg, pupilAvg)) / 10;
	} else {
		pupilThreshold = ofClamp((pupilAvg + whiteMin * 3) / 4, pupilAvg + 5, 255);
		glintThresholdForDarkEye = (maxBrightness * 3 + whiteAvg) / 4;
	}
}

void thresholdCalculator::calculateAverages(ofxCvGrayscaleAdvanced & smallCurrentImg, ofxCvGrayscaleAdvanced & maskImg, ofRectangle & targetRect) {
	
	roi.x = targetRect.x / divisor;
	roi.y = targetRect.y / divisor;
	
	maskImg.setROI(roi);
	smallCurrentImg.setROI(roi);
	
	CvScalar tempPupilAvg = cvAvg(smallCurrentImg.getCvImage(), maskImg.getCvImage());
	cvNot(maskImg.getCvImage(), notDiffImg.getCvImage());
	pupilAvg = tempPupilAvg.val[0];
	
	// get average of pupil black iteratively(get average twice) to remove the influence of glint
	cvThreshold(smallCurrentImg.getCvImage(), farFromAvg, pupilAvg + 30, 255, CV_THRESH_BINARY);		// 30 is the distance from average.
	cvSub(maskImg.getCvImage(), farFromAvg, newMask);								// make a mask to get rid of those far points.
	CvScalar newPupilAvg = cvAvg(smallCurrentImg.getCvImage(), newMask);			// get new average value.
	
	// get average, min and max value of white area of an eye.
	CvScalar tempWhiteAvg = cvAvg(smallCurrentImg.getCvImage(), notDiffImg.getCvImage());
	for (int i = 0; i < 6; i++) notDiffImg.erode();				// this might be very useful to reduce the influence of small noise & glint
	cvMinMaxLoc(smallCurrentImg.getCvImage(), &whiteMin, &whiteMax, &whiteLocMin, &whiteLocMax, notDiffImg.getCvImage());

	maskImg.resetROI();
	smallCurrentImg.resetROI();
	
	pupilAvg = newPupilAvg.val[0];					// value is in the first element of CvScalar
	whiteAvg = tempWhiteAvg.val[0];
	
}

void thresholdCalculator::calculateMinMaxBrightness(ofxCvGrayscaleAdvanced & currentImg, ofRectangle & targetRect) {
	currentImg.setROI(targetRect);
	cvMinMaxLoc(currentImg.getCvImage(), &minBrightness, &maxBrightness);
	currentImg.resetROI();
}



float	thresholdCalculator::getPupilThreshold() {
	return pupilThreshold;
}

float	thresholdCalculator::getGlintThreshold(bool isBrightEye) {
	if (isBrightEye){
		return glintThresholdForBrightEye;
	} else{
		return glintThresholdForDarkEye;
	}
}

float	thresholdCalculator::getPupilAvg() {
	return pupilAvg;
}

float	thresholdCalculator::getWhiteAvg() {
	return whiteAvg;
}

float	thresholdCalculator::getMinBrightness() {
	return minBrightness;
}

float	thresholdCalculator::getMaxBrightness() {
	return maxBrightness;
}

float	thresholdCalculator::getMinInWhite() {
	return whiteMin;
}

float	thresholdCalculator::getMaxInWhite() {
	return whiteMax;
}



void thresholdCalculator::drawPupilImageWithScanLine(int x, int y, int w, int h, ofxCvGrayscaleImage & img) {
	
	ofEnableAlphaBlending();
	
	ofPushMatrix();
	ofTranslate(x, y, 0);
	
	ofSetColor(255, 255, 255);
	img.draw(0, 0, w, h);
	
	ofSetColor(255, 255, 255,80);
	ofLine(0, scanY, w, scanY);				
	ofLine(scanX, 0, scanX, h);
	
	ofSetColor(255, 0, 0, 50);
	ofFill();
	ofCircle(whiteLocMin.x * (img.width / roi.width), whiteLocMin.y * (img.height / roi.height), 10);
	
	ofDisableAlphaBlending();
	
	ofSetColor(255, 255, 255);
	ofDrawBitmapString("imgBeforeThreshold", 1, h + 12);
	
	ofPopMatrix();
	
}

void thresholdCalculator::drawBrightnessScanGraph(int x, int y, ofxCvGrayscaleImage & img, bool bIsVertical, float threshold_p,
												  float threshold_g, string graphname)
{
	unsigned char * tempPixels = img.getPixels();
	
	ofPushMatrix();
	ofTranslate(x, y, 0);
	
	ofSetColor(255, 255, 255, 100);
	ofBeginShape();
	
	if (!bIsVertical) {
		int nLine = scanY;
		for (int i = 0; i < img.width; i++){
			ofVertex(i, 255 - (int)tempPixels[i + img.width * nLine]);
		}
	} else {
		int nLine = scanX;
		for (int i = 0; i < img.height; i++){
			ofVertex(i, 255 - (int)tempPixels[nLine + i * img.width]);
		}
	}
	
	ofEndShape(false);
	
	ofSetColor(255, 0, 0, 80);
	ofLine(0, 255 - threshold_p, img.width, 255 - threshold_p);
	
	ofSetColor(0, 0, 255, 80);
	ofLine(0, 255 - threshold_g, img.width, 255 - threshold_g);
	
	ofSetColor(120, 120, 120, 80);
	ofLine(0, 255 - getMinInWhite(), img.width, 255 - getMinInWhite());
	ofLine(0, 255 - getPupilAvg(), img.width, 255 - getPupilAvg());
	
	ofSetColor(120, 120, 255, 80);
	ofLine(0, 255 - getGlintThreshold(true), img.width, 255 - getGlintThreshold(true));
	
		
	ofSetColor(255, 255, 255);
	ofRect(0, 0, img.width, img.height);
	ofDrawBitmapString(graphname, 1, 255 + 12);
	
	ofPopMatrix();
	
}


