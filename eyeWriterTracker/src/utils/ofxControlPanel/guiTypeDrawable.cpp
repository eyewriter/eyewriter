#include "guiTypeDrawable.h"

//----------------------------------------------------------
//----------------------------------------------------------
void drawableStacker::addDrawer(ofBaseDraws * drawer){
	drawers.push_back(drawer);
}

void drawableStacker::setWidth(float w){
	width = w;
}

void drawableStacker::setHeight(float h){
	height = h;
}

float drawableStacker::getWidth(){
	return width;
}

float drawableStacker::getHeight(){
	return height;
}

void drawableStacker::draw(float x, float y, float w, float h){
	ofSetColor(0xFFFFFF);
	for(unsigned int i = 0; i < drawers.size(); i++){
		drawers[i]->draw(x, y, w, h);
	}
}

void drawableStacker::draw(float x, float y){
	ofSetColor(0xFFFFFF);
	for(unsigned int i = 0; i < drawers.size(); i++){
		drawers[i]->draw(x, y, width, height);
	}
}


//----------------------------------------------------------
//----------------------------------------------------------

guiTypeDrawable::guiTypeDrawable(){
	vid = NULL;
	bgColor.setSelectedColor(0, 0, 0, 255);
	bgColor.setColor(0, 0, 0, 255);
}

//------------------------------------------------
void guiTypeDrawable::setup(string videoName, ofBaseDraws * vidIn, float videoWidth, float videoHeight){
	vid = vidIn;
	name = videoName;
	updateText();

	setDimensions(videoWidth, videoHeight);

}

//-----------------------------------------------.
void guiTypeDrawable::updateGui(float x, float y, bool firstHit, bool isRelative){

}

//-----------------------------------------------.
void guiTypeDrawable::render(){
	ofPushStyle();

		glPushMatrix();
		//glTranslatef(boundingBox.x, boundingBox.y, 0);
			guiBaseObject::renderText();

			//draw the background
			ofFill();
			glColor4fv(bgColor.getColorF());
			ofRect(hitArea.x, hitArea.y, hitArea.width, hitArea.height);

			ofDisableAlphaBlending();

			ofSetColor(0xFFFFFF);
			vid->draw(hitArea.x, hitArea.y, hitArea.width, hitArea.height);

		glPopMatrix();

	ofPopStyle();
}


