/*
 *  guiCallbackValue.h
 *  ofxControlPanelDemo
 *
 *  Created by theo on 31/03/2010.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#pragma once

#include "ofMain.h"

class guiCallbackData{
	public:
		
		string elementName;
		string groupName;
		
		void setup(string groupNameIn, string elementNameIn = ""){
			groupName	= groupNameIn;
			elementName = elementNameIn;
		}
		
		void addFloat(float val){
			fVal.push_back(val);
		}

		void addInt(int val){
			iVal.push_back(val);
		}

		void addString(string val){
			sVal.push_back(val);
		}
		
		float getFloat(int which){
			if( which < fVal.size() ){
				return fVal[which];
			}else{
				return 0.0;
			}		
		}

		float getInt(unsigned int which){
			if( which < iVal.size() ){
				return iVal[which];
			}else{
				return 0;
			}
		}

		string getString(int which){
			if( which < sVal.size() ){
				return sVal[which];
			}else{
				return "";
			}
		}
		
		vector <string> sVal;
		vector <float> fVal;
		vector <int> iVal;		
};


class guiCallbackValue{
	public:
		guiCallbackData val;
};

