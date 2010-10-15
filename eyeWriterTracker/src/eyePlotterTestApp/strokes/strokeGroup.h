/*
 *  strokeGroup.h
 *  openFrameworks
 *
 *  Created by theo on 17/08/2009.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */
#pragma once
#include "stroke.h"
#include "ofxPoint2f.h"
#include "groupStyle.h"

class strokeGroup{

	public:
	
		strokeGroup(){
			newStrokeOnNext = true;
			isCurve = false;
		}
		
		void undo(){
			if(strokes.size())strokes.pop_back();
			newStrokeOnNext = true;
		}
		
		void undoPoint(){
			if(strokes.size()){
				strokes.back().undo();
			}
		}
		
		void scale(float sx, float sy){
	
			float nWidth  = rect.width  * sx ;
			float nHeight = rect.height * sy ;

			ofRectangle newRect( rect.x, rect.y, nWidth, nHeight );
			newRect.x -= (nWidth-rect.width)  * 0.5;
			newRect.y -= (nHeight-rect.height) * 0.5;
			
			for(int k = 0; k < strokes.size(); k++){
			
				for(int i = 0; i < strokes[k].pts.size(); i++){
					strokes[k].pts[i].x -= rect.x;
					strokes[k].pts[i].y -= rect.y;
					strokes[k].pts[i].x /= rect.width;
					strokes[k].pts[i].y /= rect.height;
					
					strokes[k].pts[i].x *= newRect.width;
					strokes[k].pts[i].y *= newRect.height;	
					strokes[k].pts[i].x += newRect.x;
					strokes[k].pts[i].y += newRect.y;			
				}
				strokes[k].updateBoundingRect();

			}
			
			updateBoundingRect();
		}

		float rotate(float angle){

			ofxPoint2f mid(rect.x + rect.width/2, rect.y + rect.height/2);
			ofxPoint2f tmp;
			
			for(int k = 0; k < strokes.size(); k++){
				for(int i = 0; i < strokes[k].pts.size(); i++){
					tmp.set(strokes[k].pts[i].x, strokes[k].pts[i].y);
					tmp.rotate(angle, mid);
					strokes[k].pts[i].x = tmp.x;
					strokes[k].pts[i].y = tmp.y;
				}
				strokes[k].updateBoundingRect();
			}
			
			updateBoundingRect();
		}

		void shift(float x, float y){
			
			for(int k = 0; k < strokes.size(); k++){
				for(int i = 0; i < strokes[k].pts.size(); i++){
					strokes[k].pts[i].x += x;
					strokes[k].pts[i].y += y;
				}
				strokes[k].updateBoundingRect();
			}
			
			updateBoundingRect();
		}

			
		void setIsCurve(bool bCurve){
			isCurve = bCurve;
			if( strokes.size() ){
				strokes.back().setIsCurve(isCurve);
			}
		}			
		
		void begin(){
			newStrokeOnNext = true;
		}
		
		float getStartTime(){
			if( strokes.size() ){
				if( strokes[0].pts.size() ){
					return strokes[0].pts[0].t;
				}
			}
			return 0.0;
		}
		
		void addPoint(float x, float y, float t, float c){
			
			if(strokes.size() == 0){
				newStrokeOnNext = true;
			}
			
			if( newStrokeOnNext ){
				strokes.push_back(stroke());
				if( isCurve ){
					strokes.back().setIsCurve(isCurve);
				}
				newStrokeOnNext = false;
			}
			strokes.back().addPoint(x, y, t, c);
			updateBoundingRect();
		}
		
		void nextStroke(){
			newStrokeOnNext = true;
		}	
		
		void clear(){
			strokes.clear();
		}
		
		ofPoint getLastPoint(){
			if( hasPoints() ){
				return (ofPoint)( strokes.back().pts.back() );
			}
		}

		int getLastStrokeNumPoints(){
			int num = 0;
			
			if( strokes.size() ){
				return strokes.back().pts.size();
			}	
			
			return num;
		}
		
		int getTotalNumPoints(){
			int num = 0;
			for(int k = 0; k < strokes.size(); k++){
				num += strokes[k].pts.size();
			}
			return num;
		}
		
		bool hasPoints(){
			return (strokes.size() && strokes.back().pts.size() );
		}
		
		void updateBoundingRect(){
			ofRectangle boundingRect;
			
			float minX, minY, maxX, maxY = 0;
			
			for(int i = 0; i < strokes.size(); i++){
				if( !strokes[i].hasPoints() )continue;
			
				rect = strokes[i].getBoundingRect();
				
				if( rect.width == 0 || rect.height == 0 )continue;

				if( i == 0 ){
					minX = rect.x;
					minY = rect.y;
					maxX = rect.width + minX;
					maxY = rect.height + minY;
				}else{
					
					if( rect.x < minX ){
						minX = rect.x;
					}
					
					if( rect.x + rect.width > maxX ){
						maxX = rect.x + rect.width;
					}
					
					if( rect.y < minY ){
						minY = rect.y;
					}
					
					if( rect.y + rect.height > maxY ){
						maxY = rect.y + rect.height;
					}
				}
					
			}			
			
			rect = ofRectangle(minX, minY, maxX - minX, maxY - minY);
		}
		
		ofRectangle getBoundingRect(){
			return rect;
		}
		
		void drawBoundingRect(){
			updateBoundingRect();
			ofPushStyle();
				ofNoFill();
				ofRect( rect.x, rect.y, rect.width, rect.height);
			ofPopStyle();
		}
		
		void draw(bool lastCircle = true){
			for(int i = 0; i < strokes.size(); i++){
				strokes[i].draw(lastCircle);
			}
		}
		
		bool isCurve;
		bool newStrokeOnNext;
		ofRectangle rect;
		vector <stroke> strokes;
		groupStyle style;
				
};