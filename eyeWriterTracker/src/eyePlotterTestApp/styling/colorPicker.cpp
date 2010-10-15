/*
 *  colorPicker.cpp
 *  openFrameworks
 *
 *  Created by Keith on 8/22/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#include "colorPicker.h"

extern int buttonCount;

template<class T>
std::string toString(const T& t)
{
	std::ostringstream stream;
	stream << t;
	return stream.str();
}

colorPicker::colorPicker(){};

void colorPicker::setup(int xx, int yy){
	
	if( colorXML.loadFile( "colors.xml" ) ){
	   numLibraries = colorXML.getNumTags( "LIBRARY" );
		string libName;
		for ( int i = 0; i < numLibraries; i++ ){
			vector<int> cols;
			colorXML.pushTag("LIBRARY");
			libName = colorXML.getValue("NAME","",0);
			int numColors = colorXML.getNumTags("COLOR");
			for ( int c = 0; c < numColors; c++ ){
				cols.push_back(colorXML.getValue("COLOR",0,c));
			}
			libraries[libName] = cols;
			cols.clear();
		}
	}
	
	//a bit hardcoded for now...
	width = 90;
	x = xx; y = yy;
	outColor = -1;
	butPerRow = 6;
	butPerPage = butPerRow * 5;

	curLibrary = "Molotow";
	curPage = 0;
	maxColors = libraries[curLibrary].size() - 1;
	maxPages = floor((libraries[curLibrary].size()-1) / butPerPage);
	
	for ( int i = 0 ; i < butPerPage; i++ ){
		colorButtons.push_back( buttonTrigger() );
		colorButtons.back().setup( "", width * ( i % butPerRow ), width * ( i / butPerRow ), width, width);
		colorButtons.back().setMaxCounter( buttonCount * 3 );
		colorButtons.back().setRetrigger(false);	
	}
	
	nextPage.setup("nextPage", width * (butPerRow-1), width * butPerPage / butPerRow, width, width);
	nextPage.setMaxCounter(buttonCount * 3);
	nextPage.setRetrigger(true);	
	
	prevPage.setup("prevPage", 0, width * butPerPage / butPerRow, width, width);
	prevPage.setMaxCounter(buttonCount * 3);
	prevPage.setRetrigger(true);	
}

int colorPicker::update(int mx, int my){
	if(nextPage.update( mx - x, my - y)) {
		( curPage < maxPages ) ? curPage++ : curPage = 0;
	}
	if(prevPage.update( mx - x, my - y)) {
		( curPage > 0 ) ? curPage-- : curPage = maxPages;
	}
	
	for (int i = 0 ; i < colorButtons.size(); i++){
		if(colorButtons[i].update(mx - x, my - y)){
			if (i+curPage*butPerPage < maxColors) outColor = libraries[curLibrary][i+curPage*butPerPage];
			else outColor = -1;
		}
	}
	return outColor;
}

void colorPicker::draw(int xx, int yy){
	xx != x ? x = xx : x=x;
	yy != y ? y = yy : y=y;
	
	glPushMatrix();
	glTranslatef(xx,yy,0);
	nextPage.draw(255);
	prevPage.draw(255);
	for (int i = 0; i < colorButtons.size(); i++){
		if (i+curPage*butPerPage < maxColors) {
			colorButtons[i].draw( toString(libraries[curLibrary][i+curPage*butPerPage]) );
		}
	}
	glPopMatrix();

}
