/*
 *  guiTypeVideo.h
 *  ofxControlPanelDemo
 *
 *  Created by theo on 23/03/2010.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "guiBaseObject.h"
#include "guiColor.h"
#include "simpleColor.h"
#include "guiValue.h"
#include "guiTypeDrawable.h"

class guiTypeVideo : public guiTypeDrawable{
	
	public:
		 //------------------------------------------------
        void setup(string videoName, ofVideoPlayer * vidIn, float videoWidth, float videoHeight);
        void updateGui(float x, float y, bool firstHit, bool isRelative);
        void render();
		
		float pButtonX, pButtonY, pButtonW, pButtonH;	
		float scX, scY, scW, scH;	
		
		float scrubPct;
		bool playPause;
		ofVideoPlayer * video;
};
