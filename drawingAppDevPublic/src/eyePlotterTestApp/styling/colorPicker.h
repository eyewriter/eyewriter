/*
 *  colorPicker.h
 *  openFrameworks
 *
 *  Created by Keith on 8/22/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#pragma once

#include "ofGraphics.h"
#include "buttonTrigger.h"
#include "ofxXmlSettings.h"
#include <string>
#include <map>

using namespace std;

class colorPicker{
	public:
	
	//vector < int > colors;  // deprecated
	map<string,vector <int> > libraries;
	
	vector <buttonTrigger> colorButtons;
	buttonTrigger nextPage;
	buttonTrigger prevPage;
	
	int width, x, y;
	
	int outColor;
	ofxXmlSettings colorXML;
	int numLibraries;
	int butPerPage;
	int butPerRow;
	int curPage;
	int maxPages;
	int maxColors;
	string curLibrary;
	
	
	colorPicker();
	void setup(int xx, int yy);
	
	int update(int mx, int my);
	void draw(int xx, int yy);
};