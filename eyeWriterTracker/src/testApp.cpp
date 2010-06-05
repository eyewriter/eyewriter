
#include "testApp.h"
#include "stdio.h"



//--------------------------------------------------------------
testApp::testApp(){
	
}                                                  

//--------------------------------------------------------------
void testApp::setup(){
	
	
	//---- setup standard application settings
	
	ofSetVerticalSync(true);  
	
	mode = MODE_TRACKING;
	TM.setup();
	CM.setup();
	
	eyeSmoothed.set(0,0,0);
	
	
	BT.setup("catch me!", 50,50,180,180);
	
}

//--------------------------------------------------------------
void testApp::update(){
	
	ofBackground(70,70,70);
	
	// update the tracking manager (and internally, its input manager)
	TM.update();
	
	// update the calibration manager
	CM.update();
	
	// record some points if we are in auto mode
	if (CM.bAutomatic == true && CM.bAmInAutodrive == true && CM.bInAutoRecording){
		
		if (TM.bGotAnEyeThisFrame()){	
			ofPoint trackedEye = TM.getEyePoint();
			CM.registerCalibrationInput(trackedEye.x,trackedEye.y);
		}
	}
	
	// smooth eye data in...
	if (CM.bBeenFit){									// become true after calibration
		ofPoint trackedEye = TM.getEyePoint();
		ofPoint screenPoint = CM.getCalibratedPoint(trackedEye.x, trackedEye.y);
		eyeSmoothed.x = CM.smoothing * eyeSmoothed.x + (1-CM.smoothing) * screenPoint.x;
		eyeSmoothed.y = CM.smoothing * eyeSmoothed.y + (1-CM.smoothing) * screenPoint.y;
	}
	
	if (mode == MODE_TEST){
		ofPoint pt = eyeSmoothed;
		if (BT.update(pt.x, pt.y)){
			BT.x = ofRandom(100,ofGetWidth()-100);
			BT.y = ofRandom(100,ofGetHeight()-100);
		}
	}
}

//--------------------------------------------------------------
void testApp::draw(){
	
	
	ofSetColor(255, 255, 255);
	
	if (mode == MODE_TRACKING)			TM.draw();
	if (mode == MODE_CALIBRATING)		CM.draw();
	if (mode == MODE_TEST)				BT.draw();
	
//	if (mode != MODE_TRACKING)	TM.drawBrightDarkPupil(0, 0, TM.tracker.targetWidth, 0);
	if (mode != MODE_TRACKING) TM.drawInput(0, 0, TM.IM.width/4, TM.IM.height/4, TM.IM.width/4, 0, TM.IM.width/4, TM.IM.height/4);
	
	// draw a green dot to see how good the tracking is:
	if (CM.bBeenFit){
		ofSetColor(0,255,0,120);
		ofFill();
		ofCircle(eyeSmoothed.x, eyeSmoothed.y, 20);
	}
	
}



//--------------------------------------------------------------
void testApp::keyPressed(int key){
	
	
	switch (key){
			
		case	OF_KEY_RETURN:
			mode ++;
			mode %= 3; // number of modes;
			break;
			
		case	'f':
		case	'F':
			ofToggleFullscreen();
			break;
	}
	
	if (mode == MODE_CALIBRATING){
		CM.keyPressed(key);
	}
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){
	
}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){
	
}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){
	
	if (mode == MODE_TRACKING)			TM.mouseDragged(x, y, button);
	if (mode == MODE_CALIBRATING)		CM.mouseDragged(x, y, button);
	
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
	
	
	if (mode == MODE_TRACKING)			TM.mousePressed(x, y, button);
	if (mode == MODE_CALIBRATING)		CM.mousePressed(x, y, button);
	
	
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){
	
	if (mode == MODE_TRACKING)			TM.mouseReleased();
	if (mode == MODE_CALIBRATING)		CM.mouseReleased(x,y,button);
	
}


//--------------------------------------------------------------
void testApp::resized(int w, int h){
	
}


