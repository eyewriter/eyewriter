#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
	
	//we fake fps in update
	ofSetFrameRate(8.0);
	ofSetVerticalSync(true);
	
	eyeApp.setup();	
}

//--------------------------------------------------------------
void testApp::update(){

	float mousex	= mouseX + ofRandom(-2.9, 2.9);
	float mousey	= mouseY + ofRandom(-2.9, 2.9);

	//aprox 10 fps
	//if( ofGetFrameNum() % 6 ==  0 ){
		eyeApp.update(mouseX, mouseY);
	//}
		
}


//--------------------------------------------------------------
void testApp::draw(){
	eyeApp.draw();	
}

//--------------------------------------------------------------
void testApp::keyPressed  (int key){
	
	
	if(key == 'f' ){
		ofToggleFullscreen();
	}else{
		eyeApp.keyPressed(key);
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
	
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){
	
}

//--------------------------------------------------------------
void testApp::resized(int w, int h){

}

