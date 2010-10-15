#ifndef _TRACKSCENE_H
#define _TRACKSCENE_H

#include "ofMain.h"
#include "eyeTracker.h"
#include "ofxControlPanel.h"
#include "inputManager.h"
#include "threadedImgSaver.h"
#include "ofxFileHelper.h"
#include "ofxXmlSettings.h"
#include "ofxDirList.h"

class trackingManager {
	
public:
	trackingManager();
	~trackingManager();
	
	void setup();
	void update();
	void draw();
	void drawInput(int xBright, int yBright, int wBright, int hBright, int xDark, int yDark, int wDark, int hDark);
	void drawRawInput(int offsetX, int offsetY, float scale);	
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased();
	void setupGui();
	void updateGui();
	void trackEyes();
	void videoSettings();		// open video settings panel if using a camera
	ofPoint getEyePoint();		// returns the tracked vector from glint to pupil centroid
	bool bGotAnEyeThisFrame();
	void setOriginalPosition();
	
	inputManager		IM;
	ofxControlPanel		panel;	
	eyeTracker			tracker;
	
	bool				bFoundEye;
	bool				bDrawRawInput;
	bool				bFocusScreenMode;
	
	ofPoint				eyePoint;
	ofPoint				glintPoint[2];
	ofPoint				glintPupilVector;
	ofPoint				currentdrawPoint;
	vector <ofPoint>	trail;
	
	ofImage				originalPositionB;
	ofImage				originalPositionD;
	bool				bOriginalPositon;
	
	// ideally we would have hasValueChanged() in ofxControlPanel
	float lastShutter, lastGain, lastExposure, lastBrightness, lastGamma;
};

#endif
