#include "inputManager.h"




/*
<app>

	<!-- mode, input 0 = live, 1 = video -->
	<mode>1</mode>

	<!-- some per mode settings -->
	<videoFile>inputTestVideos/test1.mov</videoFile>
 
	<videoGrabber>
		<width>640</width>
		<height>480</height>
		
		<!-- this is optional -->
		<deviceId></deviceId>
	</videoGrabber>


</app>
*/

inputManager::inputManager() :
		mode(0),
		grabberType(0),
		vidGrabber(NULL) {
}

inputManager::~inputManager() {
	delete vidGrabber;
}

//--------------------------------------------------------------
void inputManager::setup(){
	
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
	}
	
	if (mode == INPUT_LIVE_VIDEO){
		//cout << "input manager: grabbing video" << endl;
		grabberType = XML.getValue("app:videoGrabber:grabberType", 0);
		width = XML.getValue("app:videoGrabber:width", 640);
		height = XML.getValue("app:videoGrabber:height", 480);
		int device = XML.getValue("app:videoGrabber:deviceId", 0);
		
		if(grabberType == INPUT_OFVIDEOGRABBER) {
			vidGrabber = new ofVideoGrabber();
			ofVideoGrabber& cam = *((ofVideoGrabber*) vidGrabber);
			if (device != 0)
				cam.setDeviceID(device);
			cam.initGrabber(width, height, false);	// false = no texture. faster...
			width = cam.width;
			height = cam.height;		// in case we are different then what we request... ;)
		} else if(grabberType == INPUT_OFXLIBDC) {
			vidGrabber = new ofxLibdcPtGrey();
			ofxLibdcPtGrey& cam = *((ofxLibdcPtGrey*) vidGrabber);
			// probably 640x480
			cam.setSize(width, height);
			// the firefly sensor is 752x480, so center the ROI horizontally
			cam.setPosition((INPUT_FIREFLY_WIDTH - width) / 2, 0);
			// ofxLibdc is currently grayscale only
			cam.setImageType(OF_IMAGE_GRAYSCALE);
			// use format 7 mode 0 (roi but no binning)
			cam.setFormat7(true);
			// setup firewire device 
			cam.setup(device);
			// set up the alternating strobe pattern for driving vsync
			// and embed the strobe info in the image
			cam.setupAlternatingStrobe();
			// query the camera for its max framerate at the current settings, and use that
			cam.setMaxFramerate();
		}
		
	}
	
	if (width != 0 || height != 0){
		colorImg.setUseTexture(false);
		grayImage.setUseTexture(false);
		colorImg.allocate(width,height);
		grayImage.allocate(width,height);
		grayOddImage.allocate(width,height);
		grayEvenImage.allocate(width,height);

	}

	bIsFrameNew = false;

	fcount = 0;
	
	flipX = false;
	flipY = false;
	
}

//--------------------------------------------------------------
void inputManager::update(){
	
	
	
	if (mode == INPUT_LIVE_VIDEO){
		if(grabberType == INPUT_OFVIDEOGRABBER) {
			ofVideoGrabber& cam = *((ofVideoGrabber*) vidGrabber);
			cam.grabFrame();
			bIsFrameNew = cam.isFrameNew();
		} else if(grabberType == INPUT_OFXLIBDC) {
			ofxLibdcPtGrey& cam = *((ofxLibdcPtGrey*) vidGrabber);
			cam.grabFrame();
			bIsFrameNew = cam.isFrameNew();
		}
	} else {
		vidPlayer.idleMovie();
		bIsFrameNew = vidPlayer.isFrameNew();
	}
	
	if (bIsFrameNew){

		if (mode == INPUT_LIVE_VIDEO){
			// color conversion only needs to be done for ofVideoGrabber
			if(grabberType == INPUT_OFVIDEOGRABBER) {
				ofVideoGrabber& cam = *((ofVideoGrabber*) vidGrabber);
				colorImg.setFromPixels(cam.getPixels(),width,height);
				grayImage = colorImg; // TODO: this color to gray conversion is *slow*, since it's using the CV cvt color, we can make this faster !
			} else if(grabberType == INPUT_OFXLIBDC) {
				ofxLibdcPtGrey& cam = *((ofxLibdcPtGrey*) vidGrabber);
				grayImage.setFromPixels(cam.getPixels(), width, height);
			}
		} else {
			colorImg.setFromPixels(vidPlayer.getPixels(), width,height);
			grayImage = colorImg; // TODO: this conversion is slow (see above)
		}
		
		if(mode == INPUT_LIVE_VIDEO && grabberType == INPUT_OFXLIBDC) {
			ofxLibdcPtGrey& cam = *((ofxLibdcPtGrey*) vidGrabber);
			// in the case of a ptgrey camera, we *know* which frame is which.
			fcount = cam.getEmbeddedStrobeCounter(grayImage.getPixels());
		} else {
			// otherwise we're just going to guess
			fcount++;
			fcount = fcount % 2;
		}
		
		// it's important to flip the image *after* the embedded information is read
		// otherwise the embedded information will be moved before we can read it
		if( flipX || flipY ){
			grayImage.mirror(flipY, flipX);
		}
		
		if (fcount == 0) grayEvenImage = grayImage;
		else grayOddImage = grayImage;
		
		
	}	
}

//--------------------------------------------------------------
void inputManager::drawOddFrame(int x, int y, int width, int height){	
	
	grayOddImage.draw(x,y,width, height);
	
}
//--------------------------------------------------------------
void inputManager::drawEvenFrame(int x, int y, int width, int height){	
	
	grayEvenImage.draw(x,y,width, height);
	
}			
//--------------------------------------------------------------
