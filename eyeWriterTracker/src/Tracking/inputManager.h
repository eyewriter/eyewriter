#ifndef _INPUT_MANAGER
#define _INPUT_MANAGER

#include "ofMain.h"
#include "ofxXmlSettings.h"
#include "ofxOpenCv.h"

#ifdef TARGET_OSX
#include "ofxLibdcPtGrey.h"
#endif

enum {

	INPUT_LIVE_VIDEO, INPUT_VIDEO

};

enum {
	INPUT_OFVIDEOGRABBER = 0,
	INPUT_OFXLIBDC = 1
};

#define INPUT_FIREFLY_WIDTH 752

class inputManager {

	public:
		inputManager();
		~inputManager();

		void setup();
		void update();
		void drawOddFrame(int x, int y, int width, int height);
		void drawEvenFrame(int x, int y, int width, int height);


		int						mode;
		int grabberType;

		ofBaseVideo*			vidGrabber;
		ofVideoPlayer			vidPlayer;
		int						width,height;

		ofxCvColorImage			colorImg;
        ofxCvGrayscaleImage 	grayImage;

		ofxCvGrayscaleImage		grayOddImage;
		ofxCvGrayscaleImage		grayEvenImage;


		bool					bIsFrameNew;			// on this update, is the frame new?


		int						fcount;


		bool					flipX;
		bool					flipY;


};

#endif
