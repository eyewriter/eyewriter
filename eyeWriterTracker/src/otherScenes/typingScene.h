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

extern "C" {
#include "speak.h"
};


class typingScene : public baseScene{
	
public:
	
	void setup();
	void update(float mouseX, float mouseY);
	void draw();
	void printMessageHistory();
	
	buttonTrigger	doneButton;
	
	bool bSpeakWords;
	
	
	vector <buttonTrigger> letterButtons;
	string * buttons;
	vector <string> letterButtons_lower;
	vector<string> triggeredMessageLog;
	vector<int> triggeredKeyCodes;
	//vector<string> message;
	vector<buttonToggle> actionButtons;
	string displayMessage;
	int carriageReturnCounter;
	
	ofTrueTypeFont  franklinBook;
	ofTrueTypeFont  franklinBookSmall;
	ofTrueTypeFont	verdana;
	ofTrueTypeFont  franklinBookLarge;
	bool shiftOn;
	
private:
	void typedMessage(string msg);
	
};

