#ifndef _INPUT_MANAGER
#define _INPUT_MANAGER

#include "ofMain.h"
#include "ofxXmlSettings.h"
#include "ofxOpenCv.h"

enum {
	
	INPUT_LIVE_VIDEO, INPUT_VIDEO

};




class inputManager {

	public:

		void setup();
		void update();

		int						mode;
	
		ofVideoGrabber			vidGrabber;
		ofVideoPlayer			vidPlayer;
		int						width,height;
		ofxCvColorImage			colorImg;
        ofxCvGrayscaleImage 	grayImage;
		
		bool					bIsFrameNew;			// on this update, is the frame new?
		
		
};

#endif
