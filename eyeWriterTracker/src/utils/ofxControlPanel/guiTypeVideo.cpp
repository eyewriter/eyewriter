#include "guiTypeVideo.h"

 //------------------------------------------------
void guiTypeVideo::setup(string videoName, ofVideoPlayer * vidIn, float videoWidth, float videoHeight){
	video		= vidIn;
	playPause	= false;
	
	guiTypeDrawable::setup(videoName, video, videoWidth, videoHeight);
}

//------------------------------------------------
void guiTypeVideo::updateGui(float x, float y, bool firstHit, bool isRelative){
	
	if( firstHit && state == SG_STATE_SELECTED && video != NULL ){
		if ( x >= pButtonX && x <= pButtonX + pButtonW && y >= pButtonY && y < pButtonY + pButtonH){
			
			playPause = !playPause;
							
			if( playPause )video->setPaused(true);
			else video->setPaused(false);
			
		}
	}
	
	if( state == SG_STATE_SELECTED && video != NULL ){
		if ( x >= scX && x <= scX + scW && y >= scY && y < scY + scH){
			scrubPct = ofMap(x, scX, scX + scW, 0.0, 0.99);
			video->setPosition(scrubPct);
		}
	}
	
}

//-----------------------------------------------.
void guiTypeVideo::render(){
				
	pButtonX = hitArea.x + 4;
	pButtonY =  hitArea.y + hitArea.height - 20;
	pButtonW = 16;
	pButtonH = 16;
	
	scX  = pButtonX + pButtonW + 6;
	scY  = pButtonY+1;
	scW  = hitArea.width - (pButtonW + 16);
	scH  = pButtonH-2;

	ofPushStyle();

		if( video != NULL ){
			scrubPct = video->getPosition();
		}else scrubPct = 0.0;

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
			
			ofEnableAlphaBlending();
			
			//ofSetColor(20, 90, 220, 100);
			glColor4fv( outlineColor.getColorF() );
			ofNoFill();
			ofRect(pButtonX, pButtonY, pButtonW, pButtonH);
			
			ofFill();
			
			
			glColor4fv( fgColor.getColorF() );
			if( playPause ){
				ofRect(pButtonX + 4, pButtonY + 3, 2, 9);
				ofRect(pButtonX + 9, pButtonY + 3, 2, 9);
			}else{
				float dx = pButtonX + 5;
				float dy = pButtonY + 3;
				
				ofTriangle( dx, dy, dx + 7, dy + 5, dx, dy + 10); 
			}

			glColor4fv( outlineColor.getColorF() );
			ofNoFill();
			
			//ofSetColor(20, 90, 220, 100);
			ofRect(scX, scY, scW, scH);
			
			ofFill();
			glColor4fv( fgColor.getColorF() );
			ofRect( scX + 2, scY + 2, scrubPct * (scW-4.0), scH - 4);

		glPopMatrix();

	ofPopStyle();
}
	