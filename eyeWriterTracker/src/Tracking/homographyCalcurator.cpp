#include "homographyCalcurator.h"

void homographyCalcurator::setup(int widthGlintRectangle, int heightGlintRectangle, int widthWarpedImage, int heightWarpedImage){
	
	hWidth = widthGlintRectangle;		// 100
	hHeight = heightGlintRectangle;		// 50
	
	warpedImg.allocate(widthWarpedImage, heightWarpedImage);
}

ofxCvGrayscaleAdvanced* homographyCalcurator::getWarpedImage(ofxCvGrayscaleImage& magCurrent, glintFinder& gFinder, float magRatio){
	
	// this is super rough, only warping.. think about possible fixed error related to eye position in a frame, and do it with 2 or 0 glints.
	// feature detection to get 4 point in eye picture?
	
	ofPoint averageSrcPos;
	
	srcPos[0] = gFinder.getGlintPosition(GLINT_TOP_LEFT) * magRatio;
	srcPos[1] = gFinder.getGlintPosition(GLINT_TOP_RIGHT) * magRatio;
	srcPos[2] = gFinder.getGlintPosition(GLINT_BOTTOM_LEFT) * magRatio;
	srcPos[3] = gFinder.getGlintPosition(GLINT_BOTTOM_RIGHT) * magRatio;
	
	averageSrcPos = (srcPos[0] + srcPos[1] + srcPos[2] + srcPos[3]) / 4;
	
	// Destination positions(shape) should be calibrated when should we calibrate..?
	// like, just see one point on the screen, and get the shape of the glints.
	
	dstPos[0] = ofPoint(averageSrcPos.x - hWidth / 2, averageSrcPos.y - hHeight / 2);
	dstPos[1] = ofPoint(averageSrcPos.x + hWidth / 2, averageSrcPos.y - hHeight / 2);
	dstPos[2] = ofPoint(averageSrcPos.x - hWidth / 2, averageSrcPos.y + hHeight / 2);
	dstPos[3] = ofPoint(averageSrcPos.x + hWidth / 2, averageSrcPos.y + hHeight / 2);
	
	warpedImg.warpIntoMe(magCurrent, srcPos, dstPos);
	
	return &warpedImg;
}

ofPoint* homographyCalcurator::getDstPos(){

	return dstPos;
}

void homographyCalcurator::draw(int x, int y , int w, int h){

	warpedImg.draw(x, y, w, h);
	ofDrawBitmapString("warpedImg", x + 1, y + 255 + 12);

}



