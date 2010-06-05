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
		width = XML.getValue("app:videoGrabber:width", 640);
		height = XML.getValue("app:videoGrabber:height", 480);
		int device = XML.getValue("app:videoGrabber:deviceId", 0);
		
		if (device != 0){
			vidGrabber.setDeviceID(device);
		}
		
		vidGrabber.initGrabber(width, height, false);	// false = no texture. faster...
		width = vidGrabber.width;
		height = vidGrabber.height;		// in case we are different then what we request... ;)
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
		vidGrabber.grabFrame();
		bIsFrameNew = vidGrabber.isFrameNew();
	} else {
		vidPlayer.idleMovie();
		bIsFrameNew = vidPlayer.isFrameNew();
	}
	
	if (bIsFrameNew){

		if (mode == INPUT_LIVE_VIDEO){
			colorImg.setFromPixels(vidGrabber.getPixels(),width,height);
		} else {
			colorImg.setFromPixels(vidPlayer.getPixels(), width,height);
		}
		
        grayImage = colorImg;		// TODO: this color to gray conversion is *slow*, since it's using the CV cvt color, we can make this faster !
		
		
		if( flipX || flipY ){
			grayImage.mirror(flipY, flipX);
		}
		
		fcount++;
		fcount = fcount % 2;
		
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
