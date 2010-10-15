/*
 *  strokeThumbnails.h
 *  openFrameworks
 *
 *  Created by theo on 17/08/2009.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */
#pragma once 
#include "strokeGroup.h"

class strokeThumbnails{
	public:
	
	void drawThumbnails( ofRectangle drawableRect, float spacing, vector <strokeGroup> thumbs ){
		
		if(thumbs.size() <= 1)return;
		
		ofRectangle tmpRect;
		
		float width   = drawableRect.width;
		
		float currentX = width;

		ofNoFill();

		for(int i = 0; i < thumbs.size()-1; i++){
			int k = (thumbs.size()-2)-i;
			
			tmpRect = thumbs[k].getBoundingRect();
			
			if( tmpRect.width <= 0 || tmpRect.height <= 0 ){
				continue;
			}
			
			float scaleW = tmpRect.width * ( drawableRect.height / tmpRect.height);
			
			ofSetColor(0xcccccc);
			ofRect(drawableRect.x + currentX - scaleW, drawableRect.y, scaleW, drawableRect.height);
			
			ofSetColor(33,33,33);
			ofPushMatrix();
				ofTranslate(drawableRect.x + currentX - scaleW, drawableRect.y, 0.0);
				float scaleAmnt = ( drawableRect.height / tmpRect.height);
				for(int j = 0; j < thumbs[k].strokes.size(); j++){
					
//					thumbs[k].strokes.drawAtPosWithScale(float posx, float posy, 
					
					ofBeginShape();
					for(int p = 0; p< thumbs[k].strokes[j].pts.size(); p++){
						ofVertex( scaleAmnt * (thumbs[k].strokes[j].pts[p].x - tmpRect.x), scaleAmnt * (thumbs[k].strokes[j].pts[p].y - tmpRect.y));
					}
					ofEndShape();
				}
			ofPopMatrix();
			
			currentX -= scaleW + spacing;
			if( currentX <= 0 ){
				break;
			}
		}
		
		ofFill();
		
		
		
	}
	
};