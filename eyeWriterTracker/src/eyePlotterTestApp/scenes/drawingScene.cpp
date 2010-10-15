/*
 *  drawingScene.cpp
 *  openFrameworks
 *
 *  Created by theo on 20/08/2009.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#include "drawingScene.h"

//switch to gui
extern  int buttonCount; 
extern  float rate; 
extern float ptThreshold;

//extern vector <strokeGroup>	groups;
#include "groupCollection.h"
extern groupCollection groups;

//--------------------------------------------------------------
void drawingScene::setup(){
		
	ofBackground(255, 255, 255);
	
	//toggle drawing
	bDrawing = false;
	pause.setup("recording", "paused", bDrawing, 10, 10, 130, 75);
	
	//buttons that should be shown when drawing
	nextLetter.setup("next letter", 10, 175, 130, 75);	
	nextStroke.setup("next stroke", 10, 260, 130, 60);
	undoPoint.setup("undo point", 10, 355, 130, 75);
	undo.setup("undo stroke", 10, 440, 130, 60);
	showGrid.setup("grid on", "grid off", false, 10, 200, 130, 75);
	showGrid.setMaxCounter(buttonCount);
	undoPoint.setRetrigger(true);
	
	drawableButtons.push_back(&nextLetter);
	drawableButtons.push_back(&nextStroke);
	drawableButtons.push_back(&undoPoint);
	drawableButtons.push_back(&undo);
	
	//saveSettings.loadFile("counter.xml");
	
	preFix = saveSettings.getValue("session", "tempt-day1");
	count  = saveSettings.getValue("count", 0);
	
	mx = 0.0;
	my = 0.0;

	nextStroke.setMaxCounter(buttonCount);	
	nextLetter.setMaxCounter(buttonCount);
	undo.setMaxCounter(buttonCount);
	pause.setMaxCounter(buttonCount);
	button.setWaitTime( rate );
	
	if( groups.size() == 0 ){
		groups.addGroup();
		groups.back().style.layer= 0;
	}

	
	testState = BUTTON_NONE;
}

//--------------------------------------------------------------
void drawingScene::makeNewLetter(){

	if( groups.back().getTotalNumPoints() > 1 ){
		groups.addGroup();
		groups.back().begin();
		groups.back().style = groupStyle(groups.size());
	}else{
		groups.back().clear();
	}

}

//--------------------------------------------------------------
void drawingScene::makeNewStroke(){
	if( groups.size() ){
		
		//if we have a stroke already and the last stroke has 0 or just 1 point then just clear the points
		//and don't make a new stroke - ie recycle :)
		if( groups.back().strokes.size() && groups.back().getLastStrokeNumPoints() <= 1 ){
			groups.back().strokes.back().pts.clear();
		}else{
			//otherwise make a new stroke. 
			groups.back().begin();	
		}
	}
}

//--------------------------------------------------------------
void drawingScene::update(float mouseX, float mouseY){
	
	nextStroke.setMaxCounter(buttonCount);	
	nextLetter.setMaxCounter( ((float)buttonCount) * 1.2);
	undo.setMaxCounter(buttonCount);
	pause.setMaxCounter(buttonCount);
	showGrid.setMaxCounter(buttonCount);
	
	button.setWaitTime( rate );
	
	mx = mouseX;
	my = mouseY;
	
	//UPDATE THE TOGGLE with mouse and get check toggle state
	if( pause.update(mx, my) ){
		bDrawing = pause.getState();
	}
	
	if( bDrawing ){
	
		//UPDATE THE TRIGGER returns true if triggered
		if( nextLetter.update(mx, my) ){
			makeNewLetter();
		}
		
		if( nextStroke.update(mx, my) ){
			makeNewStroke();
		}
		
		//UPDATE THE TRIGGER returns true if triggered
		if( undo.update(mx, my) ){
			groups.back().undo();
		}
		
		if( undoPoint.update(mx, my) ){
			groups.back().undoPoint();
		}
	}else{
		showGrid.update(mx, my);
	}
	
	bool checkDraw = true;
	if( bDrawing == false || (testState == BUTTON_NONE && mx < SIDE_GUI_X) ){
		checkDraw = false;
		button.clear();
	}
	
	if( checkDraw ){
		button.update(mx, my);
		bool stopped = button.isPointStationary(ptThreshold);

		if( stopped ){
			if( testState == BUTTON_NONE ){
				testState = BUTTON_STARTED;
			}else if( testState == BUTTON_STARTED ){
				testState = BUTTON_NONE;
				groups.back().nextStroke();				
			}	
		}
	}
	
	//check for adding our point 
	if( testState == BUTTON_STARTED ){
		
		//if we don't have anything to draw into - add a group
		if( groups.size() == 0 ){
			groups.addGroup();
		}	
		
		//if we have more than 2 points check to see if the current point is close enough to the first point to close it completely
		if( groups.back().getLastStrokeNumPoints() >= 3 ){
			
			ofxPoint2f point = groups.back().strokes.back().pts[0];
			ofxPoint2f mouse(mx, my);
			
			if( point.distance(mouse) <= 12 ){
				mx = point.x;
				my = point.y;
			}
			
		}
		
		//add our point
		groups.back().addPoint(mx, my, ofGetElapsedTimef(), 1.0);
		
		testState = BUTTON_NONE;
	}

	
}


//--------------------------------------------------------------
void drawingScene::draw(){
	ofPushStyle();	

	bool bGrid = showGrid.getState();

	if( bGrid ){
			float w = ofGetWidth();
			float h = ofGetHeight();
	
			ofPushStyle();

				ofSetLineWidth(1.0);
				ofEnableAlphaBlending();
				ofSetColor(0, 0, 0, 20);
		
				for(int x = SIDE_GUI_X; x < w; x+= 20){
					ofLine(x, 0, x, h);
				}
				
				for(int y = 0; y < h; y+= 20){
					ofLine(SIDE_GUI_X, y, w, y);
				}
			
			ofPopStyle();
	}

	if(bDrawing){
		for(int i = 0; i < drawableButtons.size(); i++){
			drawableButtons[i]->draw();
		}
		
		if( groups.size() && groups.back().hasPoints() && mx > SIDE_GUI_X ){
			
			if( !groups.back().newStrokeOnNext ){
				ofPoint pt = groups.back().getLastPoint();
				
				glLineStipple(1, 0x3F07 ); 
				glEnable( GL_LINE_STIPPLE ); 			
				ofLine(mx, my, pt.x, pt.y);
				glDisable(GL_LINE_STIPPLE);
			}
		}
		
	}else{
		showGrid.draw();
	}
	
	
	
	pause.draw();

	thumbnailRender.drawThumbnails(ofRectangle(240, 20, 680, 100), 8.0, groups.getVector());
	
	if( groups.size() ){

		ofEnableAlphaBlending();
		
		ofSetColor(0, 0, 0, 30);
		for(int i = 0; i < groups.size(); i++){
			groups[i].draw();
		}
	
		ofSetColor(0x000000);
		groups.back().draw();
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
	
	ofPopStyle();
}


