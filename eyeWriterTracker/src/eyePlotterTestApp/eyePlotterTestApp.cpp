#include "eyePlotterTestApp.h"

int buttonCount = 22;
float rate = 1.8;
float ptThreshold = 7.0f;

//THIS IS OUR VECTOR OF LETTERS
//vector <strokeGroup> groups;

#include "groupCollection.h"
groupCollection groups;

float timer = -5.0;
float smoothPct = 0.4;

//--------------------------------------------------------------
void eyePlotterTestApp::setup(){

	scenes.push_back(&drawing);
	scenes.push_back(&manip);
	scenes.push_back(&style);
	scenes.push_back(&saver);
	
	for(int i = 0; i < scenes.size(); i++){
		scenes[i]->setup();
	}	
	
	whichScene = 0;
	
	//Our only button at this level - goes to next scene
	nextScene.setup("NEXT MODE", 10, 580, 120, 80);
	nextScene.setMaxCounter(buttonCount);
	
	
	//GHETTO SETTINGS QUICKLY HACKED - below are the ones that worked for the first app test
	/*
		<buttonCount>28</buttonCount>
		<rate>1.400000</rate>
		<ptThreshold>7.000000</ptThreshold>
		<smoothPct>0.450000</smoothPct>
	*/
	
	ghettoSettings.loadFile("settings.xml");
	buttonCount = ghettoSettings.getValue("buttonCount", 22);
	rate = ghettoSettings.getValue("rate", 1.8);
	ptThreshold = ghettoSettings.getValue("ptThreshold", 7.0);
	smoothPct = ghettoSettings.getValue("smoothPct", 0.4);
	
	//gml.loadState(groups, "lastOne.gml");

	mx = 0;
	my = 0;
}

//--------------------------------------------------------------
void eyePlotterTestApp::update(float mouseX, float mouseY){
	
	float mousex = mouseX;
	float mousey = mouseY;
	
	mx *= (1.0-smoothPct);
	mx += smoothPct * (float)mousex;
	
	my *= (1.0-smoothPct);
	my += smoothPct * (float)mousey;	
	
	scenes[whichScene]->update(mx, my);
	
	if( nextScene.update(mouseX, mouseY) ){
		whichScene++;
		whichScene %= scenes.size();
	}
	
}


//--------------------------------------------------------------
void eyePlotterTestApp::draw(){
	ofPushStyle();

		ofFill();
		ofSetColor(10, 20, 10);
		ofRect(0, 0, SIDE_GUI_X, ofGetHeight());
				
		ofSetColor(220, 220, 220);
		ofRect(SIDE_GUI_X, 0, ofGetWidth(), ofGetHeight());	

		nextScene.draw();
	
		scenes[whichScene]->draw();
		
		
		if( ofGetElapsedTimef() - timer < 3.0 ){
			ofSetColor(0, 0, 0);
			ofDrawBitmapString("ptThresh is "+ofToString(ptThreshold, 1) + "\nrate is "+ofToString(rate, 1) 
			+ "\nbuttonCount is "+ofToString(buttonCount) 
			+ "\nsmoothPct is "+ofToString(smoothPct), 200, 200);
		}
		
	ofPopStyle();
}

//--------------------------------------------------------------
void eyePlotterTestApp::keyPressed  (int key){

	//GHETTO SETTINGS QUICKLY HACKED
	
	if(key == '=') ptThreshold += 0.8; 
	if(key == '-') ptThreshold -= 0.8; 

	if(key == ']') rate += 0.1; 
	if(key == '[') rate -= 0.1; 

	if(key == '.') buttonCount ++; 
	if(key == ',') buttonCount --; 

	if(key == 'm') smoothPct -= 0.05; 
	if(key == 'n') smoothPct += 0.05; 
	
	ptThreshold = ofClamp(ptThreshold, 2.0, 20.0);
	rate = ofClamp(rate, 0.1, 5.0);
	buttonCount = ofClamp(buttonCount, 10, 120);
	
	smoothPct = ofClamp(smoothPct, 0, 1);
	
	ghettoSettings.setValue("buttonCount", buttonCount);
	ghettoSettings.setValue("rate", rate);
	ghettoSettings.setValue("ptThreshold", ptThreshold);
	ghettoSettings.setValue("smoothPct", smoothPct);

	ghettoSettings.saveFile("settings.xml");
	
	if( key == 'e'){
	}
	
	//show settings on screen for a few seconds
	timer = ofGetElapsedTimef();
	
}

//--------------------------------------------------------------
void eyePlotterTestApp::keyReleased(int key){

}

//--------------------------------------------------------------
void eyePlotterTestApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void eyePlotterTestApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void eyePlotterTestApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void eyePlotterTestApp::mouseReleased(int x, int y, int button){
	
}

//--------------------------------------------------------------
void eyePlotterTestApp::resized(int w, int h){

}

