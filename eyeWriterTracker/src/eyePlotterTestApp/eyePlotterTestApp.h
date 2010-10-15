#pragma once

#include "ofMain.h"
#include "ofxXmlSettings.h"

#include "baseScene.h"
#include "drawingScene.h"
#include "manipulationScene.h"
#include "styleScene.h"
#include "saverScene.h"

#include "buttonTrigger.h"
#include "grafIO.h"


#include "defines.h"

class eyePlotterTestApp{

	public:

		void setup();
		void update(float mouseX, float mouseY);
		void draw();

		void keyPressed  (int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void resized(int w, int h);
		
		float mx, my;
		float preTime;
	
		int whichScene;
		vector <baseScene *> scenes;
		
		grafIO gml;
		
		styleScene style;
		drawingScene drawing;
		manipulationScene manip;
		saverScene saver;
		
		buttonTrigger nextScene;
		
		ofxXmlSettings ghettoSettings;
};

