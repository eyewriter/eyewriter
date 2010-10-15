/*
 *  buttonTrigger.h
 *  openFrameworks
 *
 *  Created by theo on 17/08/2009.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#include "ofMain.h"
#include "buttonRect.h"

class buttonTrigger : public buttonRect{

	public:

	buttonTrigger(){
		active  = false;
		hasLeft = true;
		counter = 0.0f;
		pct = 0.0f;
		maxCount = 40.0f;
		bHasFont = false;
		
	}
	
	void setDisplayFont(ofTrueTypeFont * fontIn){
		font = fontIn;
		bHasFont = true;
	}

	void setMaxCounter(float nCounter){
		maxCount = nCounter;
	}

	void setup( string onName, float xIn, float yIn, float w, float h ){
		displayText = onName;

		x		= xIn;
		y		= yIn;
		width	= w;
		height	= h;
	}

	bool update(float xIn, float yIn){

		bool changed = false;
		pct *= 0.85f;

		if( inRect(xIn, yIn) ){

			if( hasLeft ){
				counter += 0.6f;
				pct = counter;
				if(counter >= maxCount ){
					hasLeft = false;
					changed = true;
				}
			}
		}else{
			if( !hasLeft ){
				hasLeft = true;
			}else{
				counter *= 0.93f;
			}
		}

		return changed;
	}


	void draw(){
		ofFill();

		float pctActive = ofMap(pct, 0.0, maxCount, 0.0, 80.0);
		ofSetColor((int) (180 - pctActive), 180, (int) (180 - pctActive));

		ofRect(x, y, width, height);

		ofNoFill();
		ofSetColor(30, 30, 30);
		ofRect(x, y, width, height);
		
		if (!bHasFont){
			float textWidth = 8.0f * displayText.length();
			float remainX = (width - textWidth)/2;

			float textHeight = 14.0f;
			float remainY = (height - textHeight)/2.0f + (textHeight/2.0f);

			ofSetColor(100, 100, 160);
			ofDrawBitmapString(displayText, x + remainX, y + remainY);
		} else {
			ofRectangle bounds = font->getStringBoundingBox(displayText, 0,0);
			
			float textWidth = bounds.width;
			float remainX = (width - textWidth)/2;
			
			float textHeight = bounds.height;
			float remainY = (height - textHeight)/2.0f + (textHeight/2.0f);
			
			font->drawString(displayText, x + remainX, y + remainY);
		}
	}

	float maxCount;

	float pct;
	bool trigger;
	float counter;
	bool hasLeft;
	string displayText;
	bool active;
	ofTrueTypeFont * font;
	bool bHasFont;
	

};
