#include "guiTypeToggle.h"

//------------------------------------------------
void guiTypeToggle::setup(string toggleName, bool defaultVal){
	value.addValue( (int)defaultVal, 0, 1);
	name = toggleName;
}

//-----------------------------------------------
void guiTypeToggle::updateValue(){
	//CB
	notify();
}

//-----------------------------------------------.
void guiTypeToggle::updateGui(float x, float y, bool firstHit, bool isRelative){
	if(!firstHit)return;

	if( state == SG_STATE_SELECTED){
		if( value.getValueI() == 0 ){
			value.setValue(1, 0);
		}else{
			value.setValue(0, 0);
		}

		//CB
		notify();
	}
}

//-----------------------------------------------.
void guiTypeToggle::render(){
	ofPushStyle();
		glPushMatrix();
			guiBaseObject::renderText();

			//draw the background
			ofFill();
			glColor4fv(bgColor.getColorF());
			ofRect(hitArea.x, hitArea.y, hitArea.width, hitArea.height);

			//draw the outline
			ofNoFill();
			glColor4fv(outlineColor.getColorF());
			ofRect(hitArea.x, hitArea.y, hitArea.width, hitArea.height);

			if( value.getValueI() == 1){
				ofFill();
			}else{
				ofNoFill();
			}

			glColor4fv(fgColor.getColorF());
			ofRect(hitArea.x+3, hitArea.y+3, -6 + hitArea.width, -6 + hitArea.height);

		glPopMatrix();
	ofPopStyle();
}
