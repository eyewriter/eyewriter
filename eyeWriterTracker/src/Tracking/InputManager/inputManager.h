#ifndef _INPUT_MANAGER
#define _INPUT_MANAGER

#include "ofMain.h"
#include "ofxXmlSettings.h"
#include "ofxOpenCv.h"
#include "qtVideoGrabber.h"
#include "threadedImgSaver.h"
#include "ofxFileHelper.h"

#ifdef TARGET_OSX
#include "fireflyVideoGrabber.h"
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
	void startRecord();
	void stopRecord();
	
	int						width,height;
	int						mode;
	int						grabberType;
	
	baseVideoGrabber*		vidGrabber;
	ofVideoPlayer			vidPlayer;
	float					playposition;
	
	ofxCvColorImage			colorImg;
	ofxCvGrayscaleImage* 	grayImage;	
	ofxCvGrayscaleImage		grayOddImage;
	ofxCvGrayscaleImage		grayEvenImage;
	
	bool					bIsFrameNew;			// on this update, is the frame new?	
	int						fcount;
	
	bool					bRecord;
	ofxFileHelper			fileHelper;
	string					folderName;
	
	bool					flipX;
	bool					flipY;
};

#endif /* _INPUT_MANAGER */



