/*
 *  guiVariablePointer.h
 *  ofxControlPanelDemo
 *
 *  Created by theo on 01/04/2010.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */
#pragma once
#include "ofMain.h"

typedef enum{
	GUI_VAR_NONE,
	GUI_VAR_INT,
	GUI_VAR_FLOAT,
	GUI_VAR_STRING
}guiVarType;

class guiVariablePointer{
	public:
		guiVariablePointer(){
			displayName = "No name";
			ptr			= NULL;
			dataType	= GUI_VAR_NONE;
			precision   = 7;
		}
		
		guiVariablePointer( string displayNameIn, void * varPtr, guiVarType theDataType, float floatPrecision = 7){
			displayName = displayNameIn;
			ptr			= varPtr;
			dataType	= theDataType;		
			precision   = floatPrecision;
		} 
	
		ofRectangle bounds;
		string varAsString;
		string displayName;
		void * ptr;
		guiVarType dataType;
		float precision;
};