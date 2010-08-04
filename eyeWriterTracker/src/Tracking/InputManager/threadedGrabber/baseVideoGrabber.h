#pragma once

#include "ofMain.h"

class baseVideoGrabber {

public:
	
	virtual void setup(int _width, int _height, int _type){}
	virtual void update(){}
	virtual void draw(int x, int y){}
	
	ofxCvGrayscaleImage		grayImg;
	bool bNeedUpdate;
	bool bUseTexture;
	int	width, height;

};


