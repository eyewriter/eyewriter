/*
 *  manipulationScene.h
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
#include "strokeUtils.h"
#include "ofxXmlSettings.h"
#include "grafIO.h"

#include "baseScene.h"

typedef enum{
	MANIP_NONE,
	MANIP_LETTER,
	MANIP_ROTATE,
	MANIP_SHIFT,
	MANIP_ZOOM,
}manipState;

class manipulationScene : public baseScene{

	public:

		void setup();
		void update(float mouseX, float mouseY);
		void draw();
		
		buttonState				testState;
		
		int whichGroup;

		buttonTrigger selectLetter;
		buttonTrigger selectNone;
		buttonTrigger selectRotate;
		buttonTrigger selectShift;
		buttonTrigger  selectZoom;	
		
		buttonTrigger selectLeft;
		buttonTrigger selectRight;
		
		buttonTrigger rotateLeft;
		buttonTrigger rotateRight;
		
		buttonTrigger autoPlace;

		buttonTrigger shiftUp;
		buttonTrigger shiftDown;
		buttonTrigger shiftLeft;
		buttonTrigger shiftRight;
				
		buttonTrigger zoomIn;
		buttonTrigger zoomOut;
		
		bool showAll;
		
		manipState state;
								
};

