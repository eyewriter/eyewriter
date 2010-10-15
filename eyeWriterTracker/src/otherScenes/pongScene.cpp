/*
 *  typingScene.cpp
 *  openFrameworks
 *
 *  Created by andrea bradshaw on 4/10/2010.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#include "pongScene.h"

//switch to guim
extern  int buttonCount; 

//--------------------------------------------------------------
void pongScene::setup(){
	
	ballPoint.set(20,20);
	
	ballVelocity.set(ofRandom(-5,5), ofRandom(2,9));
	
	
	fasterButton.setup("FASTER", 800,100,100,100);
	fasterButton.setMaxCounter(30);
	fasterButton.setRetrigger(true);
	
	slowerButton.setup("SLOWER", 950,100,100,100);
	slowerButton.setMaxCounter(30);
	slowerButton.setRetrigger(true);
	
	
	
}
//--------------------------------------------------------------
void pongScene::update(float mouseX, float mouseY){
	float mx = mouseX;
	float my = mouseY;
	
	if (fasterButton.update(mx, my)){
		ballVelocity.x *= 1.5;
		ballVelocity.y *= 1.5;
	}
	
	if (slowerButton.update(mx, my)){
		ballVelocity.x /= 1.5;
		ballVelocity.y /= 1.5;
	}
	
	float speed = 60.0f / MAX( ofGetFrameRate() , 5);
	
	
	ballPoint.x += ballVelocity.x * speed;
	ballPoint.y += ballVelocity.y * speed;
	
	if (ballPoint.x < 0) {
		ballPoint.x = 0;
		ballVelocity.x *= -1;
	}
	if (ballPoint.x > ofGetWidth()) {
		ballPoint.x = ofGetWidth();
		ballVelocity.x *= -1;
	}
	if (ballPoint.y < 0) {
		ballPoint.y = 0;
		ballVelocity.y *= -1;
	}
	
	if (ballPoint.y > ofGetHeight()-60){
		float diff = rectPoint.x - ballPoint.x;
		if (fabs(diff) < 110/2){
			ballPoint.y = ofGetHeight()-60;
			ballVelocity.y *= -1;
		}
	}
	
	if (ballPoint.y > ofGetHeight()){
		ballPoint.x = ofRandom(0,ofGetWidth());
		ballPoint.y = 20;
		ballVelocity.set(ofRandom(-5,5), ofRandom(2,9));
		
		
		
	}
	
	
	rectPoint.x = mx;
	rectPoint.y = ofGetHeight()-40;
}	

//--------------------------------------------------------------
void pongScene::draw(){
	ofPushStyle();	
	
	ofFill();
	ofSetColor(255,255,255);
	ofCircle(ballPoint.x, ballPoint.y, 50);
	
	ofSetRectMode(OF_RECTMODE_CENTER);
	ofRect(rectPoint.x, rectPoint.y, 110, 20);
	ofSetRectMode(OF_RECTMODE_CORNER);
	
		
	ofPopStyle();
	
	fasterButton.draw();
	slowerButton.draw();
	
	//drawCursor();
}


