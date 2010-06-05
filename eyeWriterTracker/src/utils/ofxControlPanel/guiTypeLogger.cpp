#include "guiTypeLogger.h"

guiTypeLogger::guiTypeLogger(){
	log = NULL;
	pct = 0;
}

//------------------------------------------------
void guiTypeLogger::setup(string loggerName, simpleLogger * logger, float loggerWidth, float loggerHeight){
	log = logger;
	name = loggerName;
	updateText();
	
	toggle = 2;

	setDimensions(loggerWidth, loggerHeight);
	hitArea.width   = 20;
	outlineColor.selected = outlineColor.color;
}

//-----------------------------------------------.
void guiTypeLogger::updateGui(float x, float y, bool firstHit, bool isRelative){
	if( state == SG_STATE_SELECTED){
		pct = ( y - ( hitArea.y ) ) / hitArea.height;
		pct = ofClamp(pct, 0, 1);
	}
	
	if( firstHit && x >= boundingBox.x + boundingBox.width-20 && y > boundingBox.y && y<= boundingBox.y + 20 ){
		toggle++;
		if( toggle > 3 ){
			toggle = 0;
		}
	}
}

//-----------------------------------------------.
void guiTypeLogger::drawRecords(float x, float y, float width, float height){
	if( log == NULL)return;
	if( log->logs.size() == 0)return;
	
	float textH = MAX(8, displayText.getTextSingleLineHeight() + 2);
	float charW = MAX(4, displayText.stringWidth("abcdefg")/7.0);
		
	ofPushStyle();
		float yPos = textH;
		int startPos = (float)(log->logs.size()-1) * (1.0-pct);
		for(int i = startPos; i >= 0; i--){
		
			string str;
			
			if( toggle == 0 ) str = log->logs[i].logStr;
			else if( toggle == 1 ) str = log->logs[i].timeStr + log->logs[i].levelStr + log->logs[i].msg;
			else if( toggle == 2 ) str = log->logs[i].levelStr + log->logs[i].msg;
			else if( toggle == 3 ) str = log->logs[i].msg;
			
			if( str.length() * charW > width ){
				int newLen = (float)width / charW;
				str = str.substr(0, newLen);
			}

			displayText.renderString(str, x, y + yPos);

			yPos += textH;
			if(yPos+textH >= height)break;
		}
	ofPopStyle();
}

//-----------------------------------------------.
void guiTypeLogger::render(){
	ofPushStyle();

		glPushMatrix();
			//draw the background
			ofFill();
			glColor4fv(bgColor.getColorF());
			ofRect(boundingBox.x, boundingBox.y, boundingBox.width, boundingBox.height);
			
			float xx = boundingBox.x + boundingBox.width - 20;
			float yy = boundingBox.y;
			
			ofFill();
			for(int i = 0; i < 4; i++){				
				
				if( toggle == 0 || i >= toggle ){
					glColor4fv(fgColor.getSelectedColorF());
					ofFill();
					ofRect(xx, yy, 5, 10);
				}
				
				glColor4fv(outlineColor.getColorF());
				ofNoFill();
				ofRect(xx, yy, 5, 10);				
				
				xx += 5;
			}

			glColor4fv(textColor.getColorF());
			guiBaseObject::renderText();

			ofFill();
			glColor4fv(fgColor.getColorF());
			ofRect(hitArea.x, hitArea.y + (hitArea.height - 5) * pct, hitArea.width, 5);

			ofNoFill();
			glColor4fv(outlineColor.getColorF());
			ofRect(boundingBox.x, boundingBox.y, boundingBox.width, boundingBox.height);
			ofRect(hitArea.x , hitArea.y, hitArea.width, hitArea.height);

			glColor4fv(textColor.getColorF());
			if(log != NULL)drawRecords(hitArea.x+hitArea.width + 5, hitArea.y, boundingBox.width-(hitArea.width + 5), boundingBox.height);

		glPopMatrix();

	ofPopStyle();
}
