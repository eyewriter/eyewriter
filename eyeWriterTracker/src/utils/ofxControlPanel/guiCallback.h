/*
 *  guiCallback.h
 *  ofxControlPanelDemo
 *
 *  Created by theo on 31/03/2010.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#pragma once
#include "ofMain.h"
#include "guiCallbackValue.h"

class guiCustomEvent{
	public:
		vector <string> names;
		string group;
		
		ofEvent <guiCallbackData> guiEvent;		
};