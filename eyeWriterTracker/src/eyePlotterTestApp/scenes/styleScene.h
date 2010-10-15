/*
 *  manipulationScene.h
 *  openFrameworks
 *
 *  Created by keith on 20/08/2009.
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
#include "grafIO.h"

#include "baseScene.h"
#include "colorPicker.h"

typedef enum{
	STYLE_ALL,
	STYLE_LETTER,
	STYLE_STROKE,
	STYLE_FILL,
	STYLE_SHADOW,
	STYLE_OUTLINE
}styleState;

class styleScene : public baseScene{

	public:

		void setup();
		void update(float mouseX, float mouseY);
		void draw();
		void updateColor(string, int, bool);
		void updateStyle(string, int, int, int, bool, bool);
		
		int whichGroup;

		buttonTrigger selectLetter;
		buttonTrigger selectAll;
	
		buttonTrigger strokeOptions;
		buttonTrigger strokeUp;	
		buttonTrigger strokeDown;
		buttonTrigger brush;
	
		buttonTrigger fillOptions;
		buttonToggle toggleFill;
		buttonToggle toggleHoles;
	
		buttonTrigger shadowOptions;
		buttonTrigger shadowLeft;
		buttonTrigger shadowRight;
		buttonTrigger shadowUp;
		buttonTrigger shadowDown;
		buttonToggle lighting;
		buttonToggle hashes;
	
		buttonTrigger outlineOptions;
		buttonTrigger outlineUp;
		buttonTrigger outlineDown;
	
		buttonTrigger selectLeft;
		buttonTrigger selectRight;
	
		buttonToggle pickColor;
			
		int strokeWid;
	
		bool showAll, drawColors;
		
		styleState state;
		buttonState testState;
	
		colorPicker cPicker;
								
};

