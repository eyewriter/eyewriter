/*
 *  manipulationScene.cpp
 *  openFrameworks
 *
 *  Created by theo on 20/08/2009.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#include "manipulationScene.h"

//switch to gui
extern  float rate; 
extern vector <strokeGroup>	groups;
extern int buttonCount; 

//--------------------------------------------------------------
void manipulationScene::setup(){
	
	//groups.push_back(strokeGroup());
	//groups.back().begin();
		
	ofBackground(255, 255, 255);
	
	//toggle drawing
	
	selectNone.setup("Select All", 10, 10, 130, 75);
	selectLetter.setup("Select Letter", 10, 110, 130, 75);

	selectShift.setup("Select Shift", 10, 330, 130, 75);
	selectRotate.setup("Select Rotate", 10, 230, 130, 75);
	selectZoom.setup("Select Zoom", 10, 430, 130, 75);
					
	drawableButtons.push_back(&selectNone);
	drawableButtons.push_back(&selectLetter);
	drawableButtons.push_back(&selectRotate);
	drawableButtons.push_back(&selectShift);
	drawableButtons.push_back(&selectZoom);
	
	rotateLeft.setup("ROTATE LEFT", 200, 20, 140, 100);
	rotateLeft.setMaxCounter(16.0);
	rotateLeft.setRetrigger(true);
	
	rotateRight.setup("ROTATE RIGHT", 360, 20, 140, 100);
	rotateRight.setMaxCounter(16.0);
	rotateRight.setRetrigger(true);

	zoomIn.setup("ZOOM IN", 200, 20, 140, 100);
	zoomIn.setMaxCounter(23.0);
	zoomIn.setRetrigger(true);
	
	zoomOut.setup("ZOOM OUT", 360, 20, 140, 100);
	zoomOut.setMaxCounter(23.0);
	zoomOut.setRetrigger(true);

	shiftUp.setup("SHIFT UP", 680, 20, 140, 100);
	shiftUp.setMaxCounter(16.0);
	shiftUp.setRetrigger(true);

	shiftDown.setup("SHIFT DOWN", 860, 20, 140, 100);
	shiftDown.setMaxCounter(16.0);
	shiftDown.setRetrigger(true);
	
	shiftLeft.setup("SHIFT LEFT", 200, 20, 140, 100);
	shiftLeft.setMaxCounter(16.0);
	shiftLeft.setRetrigger(true);

	shiftRight.setup("SHIFT RIGHT", 360, 20, 140, 100);
	shiftRight.setMaxCounter(16.0);
	shiftRight.setRetrigger(true);
	
	selectLeft.setup("< SELECT LETTER", 200, 20, 140, 100);
	selectLeft.setMaxCounter(buttonCount * 2);
	selectLeft.setRetrigger(true);

	selectRight.setup("SELECT LETTER >", 360, 20, 140, 100);
	selectRight.setMaxCounter(buttonCount * 2);
	selectRight.setRetrigger(true);	
	
	autoPlace.setup("AUTO PLACE", 860, 20, 140, 100);
	autoPlace.setMaxCounter(buttonCount * 3);
	autoPlace.setRetrigger(true);	

	showAll		= true;
	whichGroup	= 0;

	mx = 0.0;
	my = 0.0;

	
	for(int i = 0; i < drawableButtons.size(); i++){
		((buttonTrigger *)drawableButtons[i])->setMaxCounter(buttonCount);
	}
					
	state	  = MANIP_NONE;
	testState = BUTTON_NONE;
}

//--------------------------------------------------------------
void manipulationScene::update(float mouseX, float mouseY){

	selectRight.setMaxCounter(buttonCount * 2);
	selectLeft.setMaxCounter(buttonCount * 2);
	
	if( whichGroup >= groups.size() ){
		whichGroup = 0;
	}

	mx = mouseX;
	my = mouseY;

	if( selectNone.update(mx, my) ){
		state = MANIP_NONE;
		showAll = true;
	}else if( selectLetter.update(mx, my) ){
		state = MANIP_LETTER;
		showAll = false;
	}else if( selectShift.update(mx, my) ){
		state = MANIP_SHIFT;
	}else if( selectZoom.update(mx, my) ){
		state = MANIP_ZOOM;
	}else if( selectRotate.update(mx, my) ){
		state = MANIP_ROTATE;
	}
	
	if( state == MANIP_NONE ){
		if( autoPlace.update(mx, my) ){
			strokeUtils::autoPlaceAndScaleByWidth(groups, ofRectangle( SIDE_GUI_X + 50, 100, ofGetWidth() - (SIDE_GUI_X + 100), ofGetHeight()-200  ) );
		}	
	}
	
	if( state == MANIP_LETTER ){
		if( selectLeft.update(mx, my) ){
			whichGroup--;
			if( whichGroup < 0 ){
				whichGroup = groups.size()-1;
				if( whichGroup < 0 )whichGroup = 0; 
			}
		}
		
		if( selectRight.update(mx, my) ){
			whichGroup++;
			if( whichGroup >= groups.size() ){
				whichGroup = 0;
			}
		}
	}	
	
	if( state == MANIP_ROTATE ){
		if( rotateLeft.update(mx, my) ){

			float rotate = -3.0;
			int numTriggers = rotateLeft.getNumTriggers(); 

			if( numTriggers > 3 ){
				rotate = -6.0;
			}else if( numTriggers > 6 ){
				rotate = -9.0;
			}
			
			if( showAll ){
				strokeUtils::rotateGroups(groups, rotate);
			}else{
				groups[whichGroup].rotate(rotate);
			}
			
		}
		if( rotateRight.update(mx, my) ){
			float rotate = 3.0;
			int numTriggers = rotateRight.getNumTriggers(); 
			
			if( numTriggers > 3 ){
				rotate = 6.0;
			}else if( numTriggers > 6 ){
				rotate = 9.0;
			}
		
			if( showAll ){
				strokeUtils::rotateGroups(groups, rotate);
			}else{
				groups[whichGroup].rotate(rotate);
			}
		}
	}

	if( state == MANIP_ZOOM ){
	
		if( zoomIn.update(mx, my) ){
			
			float scaleIn = 1.03;
			int numTriggers = zoomIn.getNumTriggers(); 
			
			if( numTriggers > 3 ){
				scaleIn = 1.08;
			}else if( numTriggers > 6 ){
				scaleIn = 1.17;
			}
		
			if( showAll ){
				strokeUtils::scaleGroups(groups, scaleIn, scaleIn);
			}else{
				groups[whichGroup].scale(scaleIn, scaleIn);
			}			
		}
		if( zoomOut.update(mx, my) ){
		
			float scaleOut = 0.97;
			int numTriggers = zoomOut.getNumTriggers(); 

			if( numTriggers > 3 ){
				scaleOut = 0.92;
			}else if( numTriggers > 6 ){
				scaleOut = 0.83;
			}
		
			if( showAll ){
				strokeUtils::scaleGroups(groups, scaleOut, scaleOut);
			}else{
				groups[whichGroup].scale(scaleOut, scaleOut);
			}
		}
	}
	
	if( state == MANIP_SHIFT ){
		if( shiftLeft.update(mx, my) ){
		
			float shiftAmnt = -7.0;
			int numTriggers = shiftLeft.getNumTriggers(); 

			if( numTriggers > 3 ){
				shiftAmnt = -12.0;
			}else if( numTriggers > 6 ){
				shiftAmnt = -26.0;
			}
			
			if( showAll ){
				for(int i = 0; i < groups.size(); i++){
					groups[i].shift(shiftAmnt, 0);
				}
			}else{
				groups[whichGroup].shift(shiftAmnt, 0);
			}
		}
		if( shiftRight.update(mx, my) ){
			float shiftAmnt = 7.0;
			int numTriggers = shiftRight.getNumTriggers(); 

			if( numTriggers > 3 ){
				shiftAmnt = 12.0;
			}else if( numTriggers > 6 ){
				shiftAmnt = 26.0;
			}
		
			if( showAll ){
				for(int i = 0; i < groups.size(); i++){
					groups[i].shift(shiftAmnt, 0);
				}
			}else{
				groups[whichGroup].shift(shiftAmnt, 0);
			}
		}
		if( shiftUp.update(mx, my) ){
			float shiftAmnt = -7.0;
			int numTriggers = shiftUp.getNumTriggers(); 

			if( numTriggers > 3 ){
				shiftAmnt = -12.0;
			}else if( numTriggers > 6 ){
				shiftAmnt = -26.0;
			}		
		
			if( showAll ){
				for(int i = 0; i < groups.size(); i++){
					groups[i].shift(0, shiftAmnt);
				}
			}
			else{
					groups[whichGroup].shift(0, shiftAmnt);
			}
		}
		if( shiftDown.update(mx, my) ){
			float shiftAmnt = 7.0;
			int numTriggers = shiftDown.getNumTriggers(); 

			if( numTriggers > 3 ){
				shiftAmnt = 12.0;
			}else if( numTriggers > 6 ){
				shiftAmnt = 26.0;
			}		
		
			if( showAll ){
				for(int i = 0; i < groups.size(); i++){
					groups[i].shift(0, shiftAmnt);
				}
			}else{
					groups[whichGroup].shift(0, shiftAmnt);
			}
		}
	}			
	
}


//--------------------------------------------------------------
void manipulationScene::draw(){
	ofPushStyle();	

	for(int i = 0; i < drawableButtons.size(); i++){
		drawableButtons[i]->draw();
	}
		
	if( mx < SIDE_GUI_X ){
		ofPushStyle();
			ofFill();
			ofSetColor(255, 255, 255);
			ofCircle(mx, my, 9);
			ofSetColor(0,0,0);
			ofCircle(mx, my, 6);
		ofPopStyle();
	}
	else if( testState == BUTTON_NONE ){
		ofNoFill();
		ofCircle(mx, my, 9);
	}else if(testState == BUTTON_STARTED ){
		ofFill();
		ofCircle(mx, my, 5);
	}
	
	//printf("whichGroup %i\n", whichGroup);
	
	if( groups.size() ){
		for(int i = 0; i < groups.size(); i++){
			if( i == whichGroup || showAll){
				ofSetColor(0, 0, 0);
			}else{
				ofSetColor(160, 160, 160);
			}
			groups[i].draw(false);
		}
	}
	
	if(showAll){
		ofPushStyle();
		ofRectangle rect = strokeUtils::getBoundingRectFromGroups(groups);
		ofEnableAlphaBlending();
		ofNoFill();
		ofSetColor(0, 0, 0, 100);
		ofRect(rect.x, rect.y, rect.width, rect.height);
		ofPopStyle();
	}
	
	ofEnableAlphaBlending();
	if( state == MANIP_ROTATE ){
		rotateLeft.draw(50.0);
		rotateRight.draw(50.0);
	}
	if( state == MANIP_ZOOM ){
		zoomIn.draw(50.0);
		zoomOut.draw(50.0);
	}	
	if( state == MANIP_SHIFT ){
		shiftUp.draw(50.0);
		shiftLeft.draw(50.0);
		shiftRight.draw(50.0);
		shiftDown.draw(50.0);
	}	
	if( state == MANIP_LETTER ){
		selectLeft.draw(50.0);
		selectRight.draw(50.0);
	}		
	if( state == MANIP_NONE ){
		autoPlace.draw(50.0);
	}
	ofPopStyle();
}


