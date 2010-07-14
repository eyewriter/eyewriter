#ifndef _TEST_APP
#define _TEST_APP


#include "ofMain.h"


#include "trackingManager.h"
#include "calibrationManager.h"
#include "buttonTrigger.h"

enum{

	MODE_TRACKING,	MODE_CALIBRATING,	MODE_TEST

};


class testApp : public ofBaseApp {

public:

	testApp();
	void setup();
	void update();
	void draw();

	void keyPressed  (int key);
	void keyReleased(int key);
	void mouseMoved(int x, int y );
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void resized(int w, int h);

	//----- scenes

	trackingManager			TM;
	calibrationManager		CM;

	buttonTrigger			BT;


	ofPoint eyeSmoothed;
	ofPoint screenPoint;


	int mode;

	//------ drawing
	void drawHelp();
	
	static const float rotSmooth = .9;

};

#endif
