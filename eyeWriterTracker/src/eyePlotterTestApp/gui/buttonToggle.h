/*
 *  buttonToggle.h
 *  openFrameworks
 *
 *  Created by theo on 17/08/2009.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#pragma once 

#include "ofMain.h"
#include "buttonRect.h"

class buttonToggle : public buttonRect{
	
	public:
	
	buttonToggle(){
		active  = false;
		hasLeft = true;
		counter = 0.0f;
		maxCount = 10.0f;
	}
	
	void setMaxCounter(float nCounter){
		maxCount = nCounter;
	}
	
	void setup( string onName, string offName,  bool startsActive, float xIn, float yIn, float w, float h){
		displayText[1] = onName;
		displayText[0] = offName;
		active = startsActive;
		
		x		= xIn;
		y		= yIn;
		width	= w;
		height	= h;
	}

	bool update(float xIn, float yIn){
		bool changed = false;
	
		float amntShouldHappened = 60.0f;
		
		float currentTime = ofGetElapsedTimef();
		float diff = currentTime - preTime;
		if( diff <= 0.0001 ){
			diff = 0.0001;
		}
				
		float fps = ofClamp(1.0/diff, 2, 6000.0);
				
		float update = amntShouldHappened / fps;
		
		if( inRect(xIn, yIn) ){
			
			if( hasLeft ){
				counter += 0.6f*update;
				if(counter >= maxCount ){
					active  = !active;
					counter = 0.0f;
					
					hasLeft = false;
					changed = true;
				}
			}
		}else{
			if( !hasLeft ){
				hasLeft = true;
			}else{
				counter *= ofClamp(0.93f / update, 0.0, 0.99);
			}
		}

		preTime = ofGetElapsedTimef();
		
		return changed;
	}	
	
	bool getState(){
		return active;
	}
	
	void draw(float opacity = 255){
		ofFill();
		
		float pctActive = ofMap(counter, 0.0, maxCount, 0.0, 110.0);			
		if( !active ){
			ofSetColor(160 - pctActive, 160 + pctActive, 160 - pctActive, opacity);
			//ofSetColor(180 - pctActive, 180.0, 180 - pctActive);
		}else{
			ofSetColor(10 + pctActive, 240 - pctActive, 10 + pctActive, opacity);
			//ofSetColor(100 + pctActive, 180, 100 + pctActive);
		}
		ofRect(x, y, width, height); 

		ofNoFill();
		ofSetColor(30, 30, 30, opacity);
		ofRect(x, y, width, height); 

		float textWidth = 8.0f * displayText[active].length();
		float remainX = (width - textWidth)/2;

		float textHeight = 14.0f;
		float remainY = (height - textHeight)/2.0f + (textHeight/2.0f);

		ofSetColor(100, 100, 160, opacity);
		ofDrawBitmapString(displayText[active], x + remainX, y + remainY);

	}

	float preTime;
	float counter;
	bool hasLeft;
	float maxCount;
	string displayText[2];
	bool active;
	
};