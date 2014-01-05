#include "inputManager.h"

inputManager::inputManager() :
	mode(0),
	grabberType(0),
	vidGrabber(NULL) {
}

inputManager::~inputManager(){
	delete vidGrabber;
	if (mode == INPUT_VIDEO) delete grayImage;
}

void inputManager::setup(){
		
	bRecord = false;
	
	ofxXmlSettings XML;
	XML.loadFile("settings/inputSettings.xml");
	mode = XML.getValue("app:mode", 0);
	
	if (mode == INPUT_VIDEO){
		//cout << "input manager: loading video file" << endl;
		string movieToLoad = XML.getValue("app:videoFile", "");
		vidPlayer.loadMovie(movieToLoad);
		vidPlayer.play();
		width	= vidPlayer.width;
		height	= vidPlayer.height;
		playposition = 0;
		
		grayImage = new ofxCvGrayscaleImage;

		if (width != 0 || height != 0){
			grayImage->allocate(width, height);
			colorImg.allocate(width, height);
		}
	}
	
	if (mode == INPUT_LIVE_VIDEO){
		
		grabberType = XML.getValue("app:videoGrabber:grabberType", 0);
		width = XML.getValue("app:videoGrabber:width", 640);
		height = XML.getValue("app:videoGrabber:height", 480);
		int device = XML.getValue("app:videoGrabber:deviceId", 0);
		
		if(grabberType == INPUT_OFVIDEOGRABBER) {
			vidGrabber = new qtVideoGrabber;
			qtVideoGrabber& cam = *((qtVideoGrabber*) vidGrabber);
			if (device != 0) cam.setDeviceID(device);
			cam.setup(width, height, false);
			width = cam.width;				// in case we are different then what we request
			height = cam.height;
			
		} else if (grabberType == INPUT_OFXLIBDC) {
			#ifdef TARGET_OSX
			vidGrabber = new fireflyVideoGrabber;
			fireflyVideoGrabber& cam = *((fireflyVideoGrabber*) vidGrabber);
			cam.setup(width, height, false);		// can work only with grayscale camera now
			#endif
		}
		
		grayImage = &(vidGrabber->grayImg);
	}

	if (width != 0 || height != 0){
		grayOddImage.allocate(width,height);
		grayEvenImage.allocate(width,height);
		
	}
	
	flipX = false;
	flipY = false;
	fcount = 0;	
	bIsFrameNew = false;

}
	
void inputManager::update(){
		
	if (mode == INPUT_LIVE_VIDEO){

		bIsFrameNew = false;
		
		if(grabberType == INPUT_OFVIDEOGRABBER) {
			qtVideoGrabber& cam = *((qtVideoGrabber*) vidGrabber);
			if (cam.bNeedUpdate){
				bIsFrameNew = cam.bNeedUpdate;
				cam.update();
			}
		} else if (grabberType == INPUT_OFXLIBDC) {
			#ifdef TARGET_OSX
			fireflyVideoGrabber& cam = *((fireflyVideoGrabber*) vidGrabber);
			if (cam.bNeedUpdate){
				bIsFrameNew = cam.bNeedUpdate;
				cam.update();
			}
			#endif
		}

	} else {									// if (mode == INPUT_VIDEO)
		vidPlayer.idleMovie();
		bIsFrameNew = vidPlayer.isFrameNew();
		if (bIsFrameNew){
			playposition = vidPlayer.getPosition();
		}
	}
	
	if (bIsFrameNew){
		if (mode == INPUT_VIDEO){
			colorImg.setFromPixels(vidPlayer.getPixels(), width,height);
			cvCvtColor(colorImg.getCvImage(), grayImage->getCvImage(), CV_RGB2GRAY);
		}
		
		if(mode == INPUT_LIVE_VIDEO && grabberType == INPUT_OFXLIBDC) {
			#ifdef TARGET_OSX
			ofxLibdcPtGrey& cam = *((ofxLibdcPtGrey*) vidGrabber);
			// in the case of a ptgrey camera, we *know* which frame is which.
			fcount = cam.getEmbeddedStrobeCounter(grayImage->getPixels());
			#endif
		} else {
			// otherwise we're just going to guess
			fcount++;
			fcount = fcount % 2;
		}
		
		// it's important to flip the image *after* the embedded information is read
		// otherwise the embedded information will be moved before we can read it
		if( flipX || flipY ){
			grayImage->mirror(flipY, flipX);
		}

		if (fcount == 0) grayEvenImage = *grayImage;
		else grayOddImage = *grayImage;
		
		if (bRecord) {
			string temp = folderName + "/" + ofToString(ofGetFrameNum()) + ".jpg";
			threadedImageSaver * TO = new threadedImageSaver(grayImage->getPixels(), temp);
			TO->start();
		}
		
	}
}

void inputManager::drawOddFrame(int x, int y, int width, int height){
	grayOddImage.draw(x,y,width, height);
}

void inputManager::drawEvenFrame(int x, int y, int width, int height){
	grayEvenImage.draw(x,y,width, height);
}

void inputManager::startRecord(){
	
	folderName = "recordedImages/" + ofToString(ofGetMonth(),0) + ofToString(ofGetDay(), 0)
				+ ofToString(ofGetHours(), 0) + ofToString(ofGetMinutes(), 0)
				+ ofToString(ofGetSeconds(), 0);

	fileHelper.makeDirectory(folderName);
	bRecord = true;
}

void inputManager::stopRecord(){
	bRecord = false;
}



