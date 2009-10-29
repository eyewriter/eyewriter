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
#include "strokeGroup.h"
#include "buttonStateManager.h"
#include "buttonState.h"

#include "buttonRect.h"
#include "buttonToggle.h"
#include "buttonTrigger.h"
#include "strokeThumbnails.h"

#include "ofxXmlSettings.h"

#include "baseScene.h"



class drawingScene : public baseScene{

	public:

		void setup();
		void update(float mouseX, float mouseY);
		void draw();
		
		void makeNewLetter();
		void makeNewStroke();
			
		buttonState				testState;
		buttonStateManager		button;

		buttonTrigger nextStroke;
		buttonTrigger nextLetter;

		buttonToggle showGrid;

		buttonTrigger undo;
		buttonTrigger undoPoint;
		buttonToggle curveToggle;		
		
		bool bCurve;

		int		count;
		string	preFix;
		
		ofxXmlSettings saveSettings;
		ofxXmlSettings settings;
		
		float tagBeginTime;
				
		//grafIO grafSaver;
		buttonToggle pause;
		
		strokeThumbnails thumbnailRender;
		
		bool bDrawing;
};

