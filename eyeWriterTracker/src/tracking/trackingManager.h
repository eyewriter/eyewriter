#ifndef _TRACKSCENE_H
#define _TRACKSCENE_H


#include "ofMain.h"
#include "eyeTracker.h"
#include "ofxControlPanel.h"
#include "inputManager.h"

class trackingManager {
	
public:
	trackingManager();
	~trackingManager();
	
	void setup();
	void update();
	void draw();
	
	void drawInput(int xBright, int yBright, int wBright, int hBright, int xDark, int yDark, int wDark, int hDark);
	void drawEyeFinder(int x, int y, int w, int h);
	void drawEyeImageBeforePupilThreshold(int x, int y, int w, int h);
	void drawPupilFinder(int x, int y);
	void drawGlintFinder(int x, int y);
	void drawBrightDarkPupil(int xBright, int yBright, int xDark, int yDark);
	void drawAutoThresholdBrightnessGraph(int x, int y);
	void drawBrighnessScanGraph(const unsigned char * pupilpixels, int x, int y, int nLine, bool bIsVertical, string graphname);
	void drawCircles();
	
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased();
	
	void setupGui();
	void updateGui();
	
	void trackEyes();
	
	// open video settings panel if using a camera
	void videoSettings();
	
	// returns the tracked pupil centroid
	ofPoint	getEyePoint();
	bool	bGotAnEyeThisFrame();
	
	
	ofxControlPanel	panel;	
	inputManager		IM;
	eyeTracker			tracker;
	
	int					minBlob, maxBlob;
	float				threshold;
	float				threshold_bd;
	float				threshold_glint;
	
	bool				bDrawLine_Pupil;
	bool				bDrawLine_Glint;
	bool				bDrawContour;
	
	bool				bFoundEye;
	ofPoint			eyePoint;
	ofPoint			glintPoint[2];
	
	ofPoint			glintPupilVector;
	ofPoint			currentdrawPoint;
	vector <ofPoint> trail;
		
	int					scanX;
	int					scanY;
	
	
};

#endif
