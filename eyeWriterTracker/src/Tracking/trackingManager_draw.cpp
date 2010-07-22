#include "trackingManager.h"

//--------------------------------------------------------------
void trackingManager::drawInput(int xBright, int yBright, int wBright, int hBright, int xDark, int yDark, int wDark, int hDark){
	
	ofSetColor(255,255,255);
	if ((tracker.bIsBrightEye && IM.fcount == 0) || (!tracker.bIsBrightEye && IM.fcount ==1) ||
			(IM.grabberType == INPUT_OFXLIBDC && IM.mode == INPUT_LIVE_VIDEO)) { //Bright Eye => Left, Dark Eye =>Right
		IM.drawEvenFrame(xBright, yBright, wBright, hBright);
		IM.drawOddFrame(xDark, yDark, wDark, hDark);		
	} else {
		IM.drawOddFrame(xBright, yBright, wBright, hBright);
		IM.drawEvenFrame(xDark, yDark, wDark, hDark);
	}
	
	ofDrawBitmapString("Input_Bright", xBright+1, yBright+hBright + 12);			// can't display letters, if x is 0. Report the BUG.
	ofDrawBitmapString("Input_Dark", xDark+1, yDark+hDark + 12);						// can't display letters, if x is 0. Report the BUG.
	
	
}
 
//--------------------------------------------------------------
void trackingManager::drawEyeFinder(int x, int y, int w, int h){
	
	ofNoFill();
	
	ofEnableAlphaBlending();
	ofSetColor(255,255,255, 80);
	
	tracker.eFinder.currentImg.draw(x, y, w, h);
	tracker.eFinder.diffImg.draw(x, y, w, h);
	
	ofNoFill();
	
	ofSetColor(255,0,0, 130);
	ofRect(x + tracker.eFinder.boundingRect.x * w / IM.width, y + tracker.eFinder.boundingRect.y * h / IM.height,
		   tracker.eFinder.boundingRect.width * w / IM.width, tracker.eFinder.boundingRect.height * h / IM.height);
	
	ofSetColor(0,0,255, 150);
	ofRect(x + tracker.targetRect.x * w / IM.width, y + tracker.targetRect.y * h / IM.height,
		   tracker.targetRect.width  * w / IM.width, tracker.targetRect.height * h / IM.height);
	
	ofSetColor(255, 0, 0,100);
	tracker.eFinder.drawFindingRect(x, y, w, h);
	
	ofSetColor(255,255,255);
	tracker.drawEllipse(x, y, w, h);			// if the targetSize is changed, ellipseCal should be changed.
	tracker.eFinder.foundBlob.draw(x + tracker.eFinder.offsetROI.x * w / IM.width, y + tracker.eFinder.offsetROI.y * h / IM.height);
	tracker.drawGlintLine(x, y, w, h);
	
	ofDisableAlphaBlending();
	
	ofSetColor(255, 255, 255);
	ofDrawBitmapString("EyeFinder", x + 1, y + h + 12);
	
}

//--------------------------------------------------------------
void trackingManager::drawEyeImageBeforePupilThreshold(int x, int y, int w, int h){
	
	ofEnableAlphaBlending();
	
	ofSetColor(255, 255, 255);
	tracker.pFinder.imgBeforeThreshold.draw(x, y, w, h);
	
	//line for drawBrighnessScanGraph.  maybe.. shouldn't be here.
	ofSetColor(255, 255, 255,80);
	ofLine(x, y + scanY, x + w, y + scanY);				
	ofLine(x + scanX, y, x + scanX, y + h);
	
	ofDisableAlphaBlending();
	
	ofSetColor(255, 255, 255);
	ofDrawBitmapString("imgBeforeThreshold", x + 1, y + h + 12);
	
}

//--------------------------------------------------------------
void trackingManager::drawPupilFinder(int x, int y){
	
	ofEnableAlphaBlending();
	
	ofSetColor(255,255,255, 80);
	tracker.pFinder.currentImg.draw(x, y);
	tracker.pFinder.foundPupilBlob.draw(x, y);
	tracker.pFinder.ellipseCal.draw(x, y);
	
	ofDisableAlphaBlending();
	
	ofSetColor(255, 255, 255);
	ofDrawBitmapString("pupilFinder/EllipseFit", x + 1, y + tracker.pFinder.currentImg.height + 12);
	
	
}
//--------------------------------------------------------------
void trackingManager::drawGlintFinder(int x, int y){
	
	ofEnableAlphaBlending();
	
	ofSetColor(255,255,255);
	tracker.gFinder.eyeImage.draw(x, y);
	if (!tracker.gFinder.bFourGlints){
		tracker.gFinder.contourFinder.draw(x + tracker.gFinder.glintROI.x, y + tracker.gFinder.glintROI.y);
	} else {
		tracker.gFinder.contourFinder.draw(x, y);
	}
	
	ofDisableAlphaBlending();
	
	ofSetColor(255, 255, 255);
	ofDrawBitmapString("glintFinder", x + 1, y + tracker.gFinder.eyeImage.height + 12);
	
	
}
//--------------------------------------------------------------
void trackingManager::drawBrightDarkPupil(int xBright, int yBright, int xDark, int yDark){
	
	ofEnableAlphaBlending();
	
	ofSetColor(255,255,255);
	tracker.brightEyeImg.draw(xBright, yBright);
	tracker.darkEyeImg.draw(xDark, yDark);
	
	ofDisableAlphaBlending();
	
}
//--------------------------------------------------------------
void trackingManager::drawAutoThresholdBrightnessGraph(int x, int y){
	
	ofSetColor(255, 255, 255);
	ofBeginShape();
	for (int i = 0; i < tracker.averageVec.size(); i++){
		ofVertex(x + i, tracker.averageVec[i] + y);
	}	
	ofEndShape(false);
	
	ofSetColor(255, 0, 0);
	ofLine(x, tracker.pixelAvginTenframes + y, x + tracker.averageVec.size(), tracker.pixelAvginTenframes + y);
	
	ofSetColor(255, 255, 255);
	ofRect(x, y, 320, 255);
	
	ofDrawBitmapString("Threshold(Bright/Dark)", x + 1, y + 255 + 12);
	
}

//--------------------------------------------------------------
void trackingManager::drawBrighnessScanGraph(const unsigned char * pupilpixels, int x, int y, int nLine, bool bIsVertical, string graphname){
	
	ofSetColor(255, 255, 255, 100);
	ofBeginShape();
	
	if (!bIsVertical) {
		
		for (int i = 0; i < tracker.pFinder.imgBeforeThreshold.width; i++){
			ofVertex(x + i, y + 255 - (int)pupilpixels[i + tracker.pFinder.imgBeforeThreshold.width * nLine]);
		}
		
	} else {
		
		for (int i = 0; i < tracker.pFinder.imgBeforeThreshold.height; i++){
			ofVertex(x + i, y + 255 - (int)pupilpixels[nLine + i*tracker.pFinder.imgBeforeThreshold.width]);
		}
	}
	
	ofEndShape(false);
	ofSetColor(255, 0, 0, 80);
	ofLine(x, y + 255 - tracker.threshold_p, x + tracker.pFinder.imgBeforeThreshold.width, y + 255 - tracker.threshold_p);
	
	ofSetColor(0, 0, 255, 80);
	ofLine(x, y + 255 - tracker.threshold_g, x + tracker.pFinder.imgBeforeThreshold.width, y + 255 - tracker.threshold_g);
	
	ofSetColor(120, 120, 120, 80);
	ofLine(x, y + 255 - tracker.whiteAvg, x + tracker.pFinder.imgBeforeThreshold.width, y + 255 - tracker.whiteAvg);
	ofLine(x, y + 255 - tracker.pupilAvg, x + tracker.pFinder.imgBeforeThreshold.width, y + 255 - tracker.pupilAvg);
	
	ofSetColor(255, 255, 255);
	ofRect(x, y, tracker.pFinder.imgBeforeThreshold.width, tracker.pFinder.imgBeforeThreshold.height);
	
//	ofSetColor(255, 255, 0);
//	ofLine(x, y + 255 - tracker.maxBrightness, x + tracker.pFinder.imgBeforeThreshold.width, y + 255 - tracker.maxBrightness);
	
	ofSetColor(255, 255, 255);

	ofDrawBitmapString(graphname, x + 1, y + 255 + 12);
	
}

//--------------------------------------------------------------
void trackingManager::drawRawInput(int offsetX, int offsetY, float scale){
	
	ofEnableAlphaBlending();
	
	// Draw 100 red circles.
	ofSetColor(255,0,0,40);
	for (int i = 0; i < trail.size() - 1; i++) {
		ofCircle(trail[i].x * scale + offsetX, trail[i].y * scale + offsetY, 5);
	}
	
	ofSetColor(255,255,0);
	ofNoFill();
	
	ofCircle(trail[trail.size()-1].x*20 + ofGetWidth()/2, ofGetHeight()/2 + trail[trail.size()-1].y*20, 5);
	
	//ofDisableAlphaBlending();
}
//--------------------------------------------------------------
void trackingManager::drawWarpedImg(int x, int y, int w, int h){

	tracker.warpedImg.draw(x, y, w, h);
	ofDrawBitmapString("warpedImg", x + 1, y + 255 + 12);

}
//--------------------------------------------------------------




