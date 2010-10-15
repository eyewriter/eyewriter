/*
 *  buttonRect.h
 *  openFrameworks
 *
 *  Created by theo on 17/08/2009.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#pragma once 

#include "ofMain.h"

class buttonRect : public ofRectangle{
	
	public:
	
	buttonRect(){
		active  = false;
		hasLeft = true;
		counter = 0.0f;
	}
	
	virtual bool inRect(float xIn, float yIn){
		if( xIn >= x && yIn >= y && xIn <= x + width && yIn <= y + height){
			return true;
		}
		return false;
	}
	
	
	virtual bool getState(){
		return active;
	}
	
	virtual void draw(float opacitiy = 255){
	
	}
	
		
	bool active;
	bool hasLeft;
	float counter;
};