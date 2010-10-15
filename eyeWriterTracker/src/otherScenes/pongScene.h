/*
 *  drawingScene.h
 *  openFrameworks
 *
 *  Created by theo on 20/08/2009.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#pragma once

#include "ofMain.h"
#include "buttonStateManager.h"
#include "buttonState.h"

#include "buttonRect.h"
#include "buttonToggle.h"
#include "buttonTrigger.h"

#include "ofxXmlSettings.h"
#include "baseScene.h"
#include "buttonTrigger.h"



class pongScene : public baseScene{
	
public:
	
	void setup();
	void update(float mouseX, float mouseY);
	void draw();
	
	ofPoint ballPoint;
	ofPoint rectPoint;
	
	ofPoint ballVelocity;
	
	buttonTrigger fasterButton;
	buttonTrigger slowerButton;
	
	
};

