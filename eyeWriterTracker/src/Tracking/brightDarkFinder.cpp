#include "brightDarkFinder.h"

//----------------------------------------------------------------------------------------------
bool brightDarkFinder::getBrightEyeDarkEye(ofxCvGrayscaleAdvanced & img, ofRectangle & roi){
	
	img.setROI(roi);
	float	tempAvg = 	cvAvg(img.getCvImage()).val[0];
	
	averageVec.push_back(tempAvg);
	
	if (averageVec.size() > 320) averageVec.erase(averageVec.begin()); // calculation needs only 10, use 320 just for displaying.
	
	pixelAvginTenframes = 0;
	
	if (averageVec.size() > 10){
		for (int i = 0; i < 10; i++) {
			pixelAvginTenframes += averageVec[averageVec.size()-i-1];
		}
		pixelAvginTenframes /= 10;
	}
	
	img.resetROI();	
	
	if (pixelAvginTenframes < tempAvg) return true;
	else return false;
}

//----------------------------------------------------------------------------------------------
void brightDarkFinder::drawAutoThresholdBrightnessGraph(int x, int y, bool bError){
	
	ofSetColor(255, 255, 255);
	ofBeginShape();
	for (int i = 0; i < averageVec.size(); i++){
		ofVertex(x + i, averageVec[i] + y);
	}	
	ofEndShape(false);
	
	ofSetColor(255, 0, 0);
	ofLine(x, pixelAvginTenframes + y, x + averageVec.size(), pixelAvginTenframes + y);
	
	ofSetColor(255, 255, 255);
	ofRect(x, y, 320, 255);
	
	ofDrawBitmapString("Threshold(Bright/Dark)", x + 1, y + 255 + 12);
	
	if (bError) {
		ofSetColor(255, 0, 0);
		ofDrawBitmapString("Bad Alternation", x + 190, y + 255 + 12);
	
	}
	
}
//----------------------------------------------------------------------------------------------

