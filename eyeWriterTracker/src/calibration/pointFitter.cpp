#include "pointFitter.h"

void pointFitter::setup(int _nDivisionsWidth, int _nDivisionsHeight) {
	
	bBeenFit = false;
	
	nDivisionsWidth = _nDivisionsWidth;
	nDivisionsHeight = _nDivisionsHeight;
	
	nPtsInPoints.assign(nDivisionsWidth * nDivisionsHeight , 0);
	referencePoints.assign(nDivisionsWidth * nDivisionsHeight , 0);
	pointsFromls.assign(nDivisionsWidth * nDivisionsHeight , 0);

	inputCount = 6;
	outputCount = 2;

	ls.setup(inputCount, outputCount);
	lastRemoveOutliers = 0;
}

void pointFitter::startCalibration() {
	
	for (int i = 0; i < nPtsInPoints.size(); i++) {
		nPtsInPoints[i] = 0;
	}
}

void pointFitter::removeOutliers(){

	if(bBeenFit && lastRemoveOutliers != removeOutliersf) {
		ls.resetOutliers();
		ls.removeOutliers(removeOutliersf);
		lastRemoveOutliers = removeOutliersf;
	}
}

void pointFitter::update(int _currentPoint, float _xp, float _yp){
	
	xp = _xp;
	yp = _yp;
	currentPoint = _currentPoint;
}

void pointFitter::registerCalibrationInput(float x, float y){
	
	screenPoints.push_back(ofPoint(xp,yp));
	eyePoints.push_back(ofPoint(x, y));
	nPtsInPoints[currentPoint]++;
}

void pointFitter::calculate(ofRectangle & calibrationRectangle){
	
	if (eyePoints.size() < 8) return;
	if (numberThreshoodForEachPoint(2))  
	
	removePointsFarFromAverage();
	calculateWeights(eyePoints, screenPoints);
	calculateErrors(calibrationRectangle);
}

void pointFitter::removePointsFarFromAverage(){
	//check the points very far(error) from average.
	int startPoint = 0;

	vector <int> bigErrorPoints;
	
	for (int i = 0; i < nDivisionsHeight; i++){
		for (int j = 0; j < nDivisionsWidth; j++) {
			
			// make average point.
			ofPoint tempAverage;
			
			tempAverage.x = 0;
			tempAverage.y = 0;
			
			for (int k = 0; k < nPtsInPoints[j+i*nDivisionsWidth]; k++){
				tempAverage.x += eyePoints[k + startPoint].x;
				tempAverage.y += eyePoints[k + startPoint].y;
			}
			
			tempAverage /= nPtsInPoints[j+i*nDivisionsWidth];
			
			// make average distance from average point
			float distAverage = 0;
			
			for (int k = 0; k < nPtsInPoints[j+i*nDivisionsWidth]; k++){
				distAverage += ofDist(eyePoints[k + startPoint].x, eyePoints[k + startPoint].y, tempAverage.x, tempAverage.y);
			}
			
			distAverage /= nPtsInPoints[j+i*nDivisionsWidth];
			
			// check distance, check if the error is big.
			int nErrors = 0;
			
			for (int k = 0; k < nPtsInPoints[j+i*nDivisionsWidth]; k++){
				if (distAverage * 1.7 < ofDist(eyePoints[k + startPoint].x, eyePoints[k + startPoint].y, tempAverage.x, tempAverage.y)){
					int temp = k + startPoint;
					bigErrorPoints.push_back(temp);
					nErrors++;
				}
			}
			startPoint += nPtsInPoints[j+i*nDivisionsWidth];
			nPtsInPoints[j+i*nDivisionsWidth] -= nErrors;
		}
	}
	
	// remove error Points.
	for (int i = 0; i < bigErrorPoints.size(); i++) {
		//!!!: temp
		eyePoints.erase(eyePoints.begin() + bigErrorPoints[i] - i);
		screenPoints.erase(screenPoints.begin() + bigErrorPoints[i] - i);
	}
	startPoint = 0;
}

void pointFitter::calculateErrors(ofRectangle & calibrationRectangle){
	
	int startPoint = 0;

	for (int i = 0; i < nDivisionsHeight; i++) {
		for (int j = 0; j < nDivisionsWidth; j++) {
			
			int jj;
			
			if (i % 2 == 1){
				jj = nDivisionsWidth - j - 1;
			} else {
				jj = j;
			}
			
			// get reference point coodinates.
			referencePoints[j+i*nDivisionsWidth].x = calibrationRectangle.x + ((float)calibrationRectangle.width / (float)(nDivisionsWidth-1)) * jj;
			referencePoints[j+i*nDivisionsWidth].y = calibrationRectangle.y + calibrationRectangle.height
													- ((float)calibrationRectangle.height / (float)(nDivisionsHeight-1)) * i;
			
			// make average point again.
			ofPoint tempAverage;
			
			tempAverage.x = 0;
			tempAverage.y = 0;
			
			for (int k = 0; k < nPtsInPoints[j+i*nDivisionsWidth]; k++){
				tempAverage.x += eyePoints[k + startPoint].x;
				tempAverage.y += eyePoints[k + startPoint].y;
			}
			
			tempAverage /= nPtsInPoints[j+i*nDivisionsWidth];
			startPoint += nPtsInPoints[j+i*nDivisionsWidth];
			
			pointsFromls[j+i*nDivisionsWidth] = getCalibratedPoint(tempAverage.x, tempAverage.y);
			
		}
	}
}

void pointFitter::calculateWeights(vector <ofPoint> trackedPoints, vector <ofPoint> knownPoints){
	// calculate the weights for an equation that goes from tracked points to known points.

	int length = trackedPoints.size();
	
	ls.clear();
	
	for(int i = 0; i < length; i++) {
		ofPoint& ipt = trackedPoints[i];
		ofPoint& opt = knownPoints[i];
		ls.add(makeInput(ipt.x, ipt.y), makeOutput(opt.x, opt.y));
	}
	
	bBeenFit = true;
}

ofPoint	pointFitter::getCalibratedPoint (float x, float y){

	if (bBeenFit == true){
		
		vector<float> out = ls.map(makeInput(x, y));
		
		float calibratedx = out[0];
		float calibratedy =	out[1];
		
		// ---------------------------------------------------
		// let's fix "offscreen" or very bad values, since we are smoothing them in....
		// and nans, etc will screw us up bad.
		
		if (calibratedx < -1000)	calibratedx = -1000;
		if (calibratedx > 2000)		calibratedx = 2000;
		if (calibratedy < -1000)	calibratedy = -1000;
		if (calibratedy > 2000)		calibratedy = 2000;
		
		if (isnan(calibratedx))		calibratedx = 0;
		if (isnan(calibratedy))		calibratedy = 0;
		// ---------------------------------------------------
		
		return ofPoint(calibratedx, calibratedy);
	}
	return ofPoint(0,0);
}

bool pointFitter::numberThreshoodForEachPoint(int threshold){
	
	bool bEnoughValue = true;
	for (int i = 0; i < nPtsInPoints.size(); i++) {
		if (nPtsInPoints[i] < threshold) {
			bEnoughValue = false;
		}
	}
	return bEnoughValue;
}

void pointFitter::saveCalibration(){
	calibFileSaver.saveCalibration(ls, nDivisionsWidth*nDivisionsHeight, referencePoints, pointsFromls, eyePoints, screenPoints);
}

void pointFitter::loadCalibration(){
	calibFileSaver.loadCalibration(ls, referencePoints, pointsFromls, eyePoints, screenPoints);
	bBeenFit = true;
}

void pointFitter::draw(){
	
	if (bDrawLsError) drawLsError();
	if (bDrawRawCalibrationInput) drawRawCalibrationInput(rawDataOffset.x, rawDataOffset.y, rawDataScale);
}

void pointFitter::clear(){
	screenPoints.clear();
	eyePoints.clear();
	bBeenFit = false;
}

void pointFitter::drawLsError(){

	if (bBeenFit) {
		ofSetColor(0, 0, 255);
		for (int i = 0; i < nDivisionsWidth * nDivisionsHeight; i++ ){
			ofLine(referencePoints[i].x, referencePoints[i].y, pointsFromls[i].x, pointsFromls[i].y);
		}
	}
	ofSetColor(255, 255, 255);
}

void pointFitter::drawRawCalibrationInput(int offsetX, int offsetY, float scale){

	if (bBeenFit) {
		ofSetColor(0, 0, 255);
		for (int i = 0; i < eyePoints.size(); i++) {
			ofCircle(eyePoints[i].x*scale + offsetX, eyePoints[i].y*scale + offsetY, 3);
		}
	}
	ofSetColor(255, 255, 255);
}

void pointFitter::drawNonCalibration(ofRectangle & calibrationRectangle) {
	
	ofPushStyle();
	ofSetLineWidth(3);
	ofSetColor(255,0,0);
	ofNoFill();
	ofRect(calibrationRectangle.x, calibrationRectangle.y, calibrationRectangle.width, calibrationRectangle.height);
	ofPopStyle();
}

vector<float> pointFitter::makeInput(float x, float y){
	// Ax + Bx^2 + Cy + Dy^2 + Exy + F
	vector<float> in;
	in.resize(inputCount);
	in[0] = x;
	in[1] = x * x;
	in[2] = y;
	in[3] = y * y;
	in[4] = x * y;
	in[5] = 1;
	return in;
}

vector<float> pointFitter::makeOutput(float x, float y){
	vector<float> out;
	out.resize(outputCount);
	out[0] = x;
	out[1] = y;
	return out;
}

