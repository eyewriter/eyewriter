#include "calibrationManager.h"

//#include <gsl_math.h>
//#include <gsl_test.h>
//#include <gsl_fit.h>
//#include <gsl_multifit.h>
//#include <gsl_multifit_nlin.h>
//#include <gsl_blas.h>
//#include <gsl_ieee_utils.h>


//--------------------------------------------------------------
void calibrationManager::setup(){
	
	
	bBeenFit = false;
	
	calibrationInfo.loadImage("images/calibrationInfo.png");
	font.loadFont("fonts/HelveticaNeueMed.ttf", 32);
	
	nDivisionsWidth = 5;
	nDivisionsHeight = 5;
	inputCount = 6;
	outputCount = 2;
	
	ls.setup(inputCount, outputCount);
	
	nPosition = 0;
	pos  = 0;

	
	inputEnergy = 0;

	bAutomatic = false;
	bPreAutomatic = false;
	bAmInAutodrive = false;
	
	startTime = ofGetElapsedTimef();
	preTimePerDot = 10.0;
	recordTimePerDot = 0.4;
	totalTimePerDot = preTimePerDot + recordTimePerDot;
	bInAutoRecording = false;
	autoPct = 0;
	
	calibrationRectangle.x = 0;
	calibrationRectangle.y = 0;
	calibrationRectangle.width = 1024;
	calibrationRectangle.height = 768;
	
	
	panel.setup("calibration", 720, 20, 300, 400);
	panel.addPanel("auto calibration", 1, false);
	panel.addPanel("calibration setup", 1, false);
	panel.addPanel("manual calibration", 1, false);
	panel.addPanel("smoothing", 1, false);
	
	
	//---- gaze
	panel.setWhichPanel("calibration setup");
	panel.addSlider("num divisions horiz", "N_DIV_W", nDivisionsWidth, 2, 15, true);
	panel.addSlider("num divisions vert", "N_DIV_H", nDivisionsHeight, 2, 15, true);
	
	panel.addSlider("(auto) time to pre-record", "PRE_RECORD_TIME", preTimePerDot, 0.1, 5, false);
	panel.addSlider("(auto) time for record", "RECORD_TIME", recordTimePerDot, 0.1, 1.5, false);
	
	panel.setWhichPanel("auto calibration");
	
	panel.addToggle("start auto calibration", "START_AUTO", false);
	panel.addToggle("reset calibration", "RESET_CALIB", false);
	panel.addToggle("save calibration", "SAVE_CALIB", false);
	panel.addToggle("load calibration", "LOAD_CALIB", false);
	
	panel.loadSettings("settings/calibrationSettings.xml");
	
	
	panel.setWhichPanel("smoothing");
	panel.addSlider("smoothing amount", "AMOUNT_SMOOTHING", 0.97, 0.01, 1.0f, false);
	
	
	smoothing = 1.0f;
	menuEnergy = 1;
}


//--------------------------------------------------------------
void calibrationManager::start(){
	bAutomatic = true;
	bAmInAutodrive = true;
	startTime = ofGetElapsedTimef();
}


//--------------------------------------------------------------
void calibrationManager::stop(){
	bAutomatic = false;
	bPreAutomatic = false;
	bAmInAutodrive = false;
	startTime = ofGetElapsedTimef();
	nPosition = 0;
	pos  = 0;
}



//--------------------------------------------------------------
void calibrationManager::saveCalibration(){
	ofxXmlSettings xml;
	for (int i = 0; i < 6; i++){
		xml.addTag("pt");
		xml.pushTag("pt", i);
		xml.addValue("x", cxfit[i]*10000);
		xml.addValue("y", cyfit[i]*10000);
		xml.popTag();
	}
	xml.saveFile("settings/calibration.xml");
	
}

//--------------------------------------------------------------
void calibrationManager::loadCalibration(){
	
	
	
	ofxXmlSettings xml;
	xml.loadFile("settings/calibration.xml");
	for (int i = 0; i < 6; i++){
		xml.pushTag("pt", i);
		cxfit[i] = xml.getValue("x", 0.0f ) / 10000.0f;
		cyfit[i] = xml.getValue("y", 0.0f ) / 10000.0f;
		xml.popTag();
	}
	bBeenFit = true;
	
	/*
	printf("-------------------------------------------- \n");
	for (int i = 0; i < 6; i++){
		printf("cx %i = %f \n", i, cxfit[i]);
	}
	
	for (int i = 0; i < 6; i++){
		printf("cy %i = %f \n", i, cyfit[i]);
	}
	printf("-------------------------------------------- \n");
	*/
	
	
}


//--------------------------------------------------------------
void calibrationManager::update(){
	
	
	calibrationRectangle.x = 0;
	calibrationRectangle.y = 0;
	calibrationRectangle.width = ofGetWidth();
	calibrationRectangle.height = ofGetHeight();
	
	float widthPad = ofGetWidth() * 0.025f;
	float heightPad = ofGetHeight() * 0.025f;
	
	calibrationRectangle.x += widthPad;
	calibrationRectangle.y += heightPad;
	calibrationRectangle.width -= widthPad*2;
	calibrationRectangle.height -= heightPad*2;
	
	panel.update();
	
	// TODO:make it possible to change nDivisions?
//	nDivisionsWidth = panel.getValueI("N_DIV_W");
//	nDivisionsHeight = panel.getValueI("N_DIV_H");;
	
	preTimePerDot = panel.getValueF("PRE_RECORD_TIME");;
	recordTimePerDot = panel.getValueF("RECORD_TIME");;
	smoothing = panel.getValueF("AMOUNT_SMOOTHING");
	
	totalTimePerDot = preTimePerDot + recordTimePerDot;
	
	if (panel.getValueB("START_AUTO")){
		bPreAutomatic = true;
		panel.setValueB("START_AUTO", false);
	}
	
	
	
	
	if (panel.getValueB("RESET_CALIB")){
		clear();
		panel.setValueB("RESET_CALIB", false);
	}
	
	
	if (panel.getValueB("SAVE_CALIB")){
		if (bBeenFit){
			saveCalibration();
		}
		panel.setValueB("SAVE_CALIB", false);
	}
	
	if (panel.getValueB("LOAD_CALIB")){
		loadCalibration();
		panel.setValueB("LOAD_CALIB", false);
	}
	
	
	
	if ((bAutomatic == true && bAmInAutodrive == true) || bPreAutomatic){
		menuEnergy = 0.94f * menuEnergy + 0.06f * 0.0f;	
	} else {
		menuEnergy = 0.94f * menuEnergy + 0.06f * 1.0f;	
	}
	
	// do the auto: 
	
	if (bAutomatic == true && bAmInAutodrive == true){
		int nPts = nDivisionsWidth * nDivisionsHeight;
		float totalTime = totalTimePerDot * nPts;
		if (ofGetElapsedTimef() - startTime > totalTime){
			bAmInAutodrive = false;
			bInAutoRecording = false;
			calculate();
		} else {
			
			float diffTime = ofGetElapsedTimef() - startTime ;
			int pt = (int)(diffTime / totalTimePerDot);
			float diffDotTime = diffTime - pt * totalTimePerDot;
			//cout << diffTime << " " << pt <<  " " << diffDotTime << endl;
			if (diffDotTime < preTimePerDot){
				
				autoPct = (diffDotTime / preTimePerDot);
				bInAutoRecording = false;
				
				
			} else{
				
				autoPct = (diffDotTime - preTimePerDot) / recordTimePerDot;
				
				bInAutoRecording = true;
			}
			pos = pt;
		}
		
		
		
	}
	
	inputEnergy *= 0.98f;
	
	int xx = (pos % nDivisionsWidth);
	int yy = (pos / nDivisionsWidth);
	
	
	bool bEven = false;
	if (yy % 2 == 0)  bEven = true;
	
	//xp = ((float)ofGetWidth() / (float)(nDivisionsWidth-1)) * xx;
	//yp = ofGetHeight() - ((float)ofGetHeight() / (float)(nDivisionsHeight-1)) * yy;
	
	xp = bEven ? calibrationRectangle.x + ((float)calibrationRectangle.width / (float)(nDivisionsWidth-1)) * xx :
				 calibrationRectangle.x + (calibrationRectangle.width - ((float)calibrationRectangle.width / (float)(nDivisionsWidth-1)) * xx);
		 
	yp = calibrationRectangle.y + calibrationRectangle.height - ((float)calibrationRectangle.height / (float)(nDivisionsHeight-1)) * yy;
	
	
	
}


//--------------------------------------------------------------
void calibrationManager::registerCalibrationInput(float x, float y){
	screenPoints.push_back(ofPoint(xp,yp));
	eyePoints.push_back(ofPoint(x, y));	
	inputEnergy = 1;
}



//--------------------------------------------------------------
void calibrationManager::drawNonCalibration(){
	
	ofPushStyle();
	ofSetLineWidth(3);
	ofSetColor(255,0,0);
	ofNoFill();
	ofRect(calibrationRectangle.x, calibrationRectangle.y, calibrationRectangle.width, calibrationRectangle.height);
	ofPopStyle();
	
	
}


//--------------------------------------------------------------
void calibrationManager::draw(){
	
	// draw a light grid: 
	
	
	panel.draw();
	
	ofEnableAlphaBlending();
	ofSetColor(70, 70, 70, 255 - 255 *  menuEnergy);
	ofRect(0,0,ofGetWidth(), ofGetHeight());
	
	/*
	ofSetColor(255, 255, 255, 40);
	for (int i = 0; i < nDivisionsWidth; i++){
		float xp = ((float)ofGetWidth() / (float)(nDivisionsWidth-1)) * i;
		ofLine(xp, 0,xp, ofGetHeight());
	}
	
	for (int i = 0; i < nDivisionsHeight; i++){
		float yp = ofGetHeight() - ((float)ofGetHeight() / (float)(nDivisionsHeight-1)) * i;
		ofLine(0,yp, ofGetWidth(),yp);
	}
	*/
	
	ofSetColor(255, 255, 255, 40);
	for (int i = 0; i < nDivisionsWidth; i++){
		float xLine = calibrationRectangle.x + ((float)calibrationRectangle.width / (float)(nDivisionsWidth-1)) * i;
		ofLine(xLine, calibrationRectangle.y,xLine, calibrationRectangle.y + calibrationRectangle.height);
	}
	
	for (int i = 0; i < nDivisionsHeight; i++){
		float yLine = calibrationRectangle.y + calibrationRectangle.height - ((float)calibrationRectangle.height / (float)(nDivisionsHeight-1)) * i;
		ofLine(calibrationRectangle.x,yLine, calibrationRectangle.x + calibrationRectangle.width,yLine);
	}
	
	
	//-----------------------------------------------------------------------
	//-----------------------------------------------------------------------
	if (bBeenFit != true){
		
		ofEnableAlphaBlending();
		
		float rad = 30;
		glLineWidth(4);
		ofSetColor(255, 255, 255, 100);
		ofLine (xp, yp-rad, xp, yp+rad);
		ofLine (xp-rad, yp, xp+rad, yp);;
		
		
		ofSetColor(255, 255, 255, 255);

		if (bAutomatic == true && bAmInAutodrive == true && bInAutoRecording){
		} else {
			ofCircle(xp, yp, rad*0.33);
		}
		
		ofFill();
		
		// 
		// TODO: do some animation stuff here: 
		if (bAutomatic == true && bAmInAutodrive == true){
			
			if (bInAutoRecording){
				
				ofSetColor(255, 0, 0, 200);
				
				ofCircle(xp, yp, 26);
				
				ofSetColor(255, 255,255);
				ofCircle(xp, yp, 5);
				
			} else {
				
				ofSetColor(255, 255, 255, 150);
				ofCircle(xp, yp, 200 - 200* autoPct);
				
			}
			
		}
		
		
		glLineWidth(1);
		ofFill();
		
		if (!bAutomatic)
		if (inputEnergy > 0){	
			ofSetColor(255, 0, 127, 60*inputEnergy);
			ofCircle(xp, yp, rad * 3.5 );
			
			ofSetColor(255, 0, 127, 150*inputEnergy);
			ofCircle(xp, yp, rad );
		}

		ofSetColor(255, 255, 255);
		//calibrationInfo.draw(100,100);

	}
	//-----------------------------------------------------------------------
	//-----------------------------------------------------------------------
	
	
	if (!bAutomatic && !bPreAutomatic){
		
		if (!bBeenFit)	{
			ofSetColor(255, 0, 0,127);
			font.drawString("not calibrated", 100,100);
		} else {
			ofSetColor(0, 255, 0, 127);
			font.drawString("calibrated", 100,100);
		}
	}
	
	if (bPreAutomatic == true){
		calibrationInfo.draw(100,100);
	}
}

//--------------------------------------------------------------
void calibrationManager::clear(){
	screenPoints.clear();
	eyePoints.clear();
	pos = 0;
	bBeenFit = false;
}

//--------------------------------------------------------------
void calibrationManager::calculate(){
	
	
	if (eyePoints.size() < 8) return;
	
	calculateWeights(eyePoints, screenPoints);
	inputEnergy = 0;

}


//--------------------------------------------------------------
void calibrationManager::advancePosition(){
	pos++;
	pos %= (nDivisionsWidth*nDivisionsHeight);
	inputEnergy = 0;
}



//--------------------------------------------------------------
void calibrationManager::calculateWeights(vector <ofPoint> trackedPoints, vector <ofPoint> knownPoints){
	
	int length = trackedPoints.size();
		
	ls.clear();
	
	for(int i = 0; i < length; i++) {
		ofPoint& ipt = trackedPoints[i];
		ofPoint& opt = knownPoints[i];
		ls.add(makeInput(ipt.x, ipt.y), makeOutput(opt.x, opt.y));
	}
	
	ls.update();

	bBeenFit = true;
	
}


//--------------------------------------------------------------
ofPoint calibrationManager::getCalibratedPoint (float x, float y){

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


//--------------------------------------------------------------
// Ax + Bx^2 + Cy + Dy^2 + Exy + F
vector<float> calibrationManager::makeInput(float x, float y) {
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

//--------------------------------------------------------------
vector<float> calibrationManager::makeOutput(float x, float y) {
	vector<float> out;
	out.resize(outputCount);
	out[0] = x;
	out[1] = y;
	return out;
}

//--------------------------------------------------------------
void calibrationManager::mouseDragged(int x, int y, int button){
	panel.mouseDragged(x,y,button);
}

//--------------------------------------------------------------
void calibrationManager::mousePressed(int x, int y, int button){
	panel.mousePressed(x,y,button);
	
}

//--------------------------------------------------------------
void calibrationManager::mouseReleased(int x, int y, int button){
	panel.mouseReleased();
}

//--------------------------------------------------------------
void calibrationManager::keyPressed(int key){
	if (bPreAutomatic == true){
		if (key == ' '){
			bPreAutomatic = false;
			start();
		}
	}
	
	if (bPreAutomatic == false){
		if (key == ' '){
			bPreAutomatic = true;
		}
	
	}
	
	if (key == 'x'){
		clear();
		stop();
	}
}


