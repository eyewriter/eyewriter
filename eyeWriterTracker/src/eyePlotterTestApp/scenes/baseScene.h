/*
 *  baseScene.h
 *  openFrameworks
 *
 *  Created by theo on 20/08/2009.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#pragma once 

#include "ofMain.h"
#include "buttonRect.h"

#include "defines.h"

class baseScene{

	public:

		virtual void setup(){}
		virtual void update(float mouseX, float mouseY){}
		virtual void draw(){}

		vector <buttonRect *> drawableButtons;
		float mx, my;
		
				
};

