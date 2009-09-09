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
	
	
		ofxControlPanel		panel;
	
		//----- video sources... what are we tracking!
	
		inputManager		IM;

		eyeTracker			tracker;
		
		int					minBlob, maxBlob;
		float				threshold;
		bool				bFoundEye;
		ofPoint				eyePoint;
	

};

#endif
