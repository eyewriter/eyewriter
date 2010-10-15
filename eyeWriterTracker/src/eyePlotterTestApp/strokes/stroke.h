/*
 *  stroke.h
 *  openFrameworks
 *
 *  Created by theo on 17/08/2009.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */
#pragma once

#include "ofMain.h"
#include "strokePoint.h"

class stroke{

	public:
		
		stroke(){
			isCurve = false;
		}
		
		void addPoint(float x, float y, float t, float confidence = 1.0){
			pts.push_back(strokePoint(x, y, t, confidence));
			updateBoundingRect();
		}
		
		void undo(){
			if( pts.size() ){
				pts.pop_back();
				updateBoundingRect();
			}
		}
		
		void setIsCurve(bool bCurve){
			isCurve = bCurve;
		}
		
		bool hasPoints(){
			return (pts.size() > 0 );
		}	
		
		void updateBoundingRect(){
			
			if( pts.size() <= 0 ){
				rect =  ofRectangle();
			}else if( pts.size() == 1 ){
				rect = ofRectangle(pts[0].x, pts[0].y, 1, 1);
				return;
			}
			
			float minX = 9999999.0f;
			float minY = 9999999.0f;
			float maxX = -999999.0f;
			float maxY = -999999.0f;
			
			for(int i = 0; i < pts.size(); i++){
				if( pts[i].x < minX ){
					minX = pts[i].x;
				}
				if( pts[i].x > maxX ){
					maxX = pts[i].x;
				}
				if( pts[i].y < minY ){
					minY = pts[i].y;
				}
				if( pts[i].y > maxY ){
					maxY = pts[i].y;
				}
			}
					
			rect = ofRectangle(minX, minY, maxX - minX, maxY-minY);
		}
		
		ofRectangle getBoundingRect(){
			return rect;
		}
		
		void draw(bool lastCircle = true){
			ofPushStyle();
			
			if( pts.size() ){
				if( isCurve && pts.size() >= 2){
					ofNoFill();
					ofBeginShape();
						ofCurveVertex(pts[0].x, pts[0].y);
						for(int i = 0; i < pts.size(); i++){
							ofCurveVertex(pts[i].x, pts[i].y);
						 } 
						ofCurveVertex(pts.back().x, pts.back().y);
					ofEndShape();
				}else{
					ofNoFill();
					ofBeginShape();
						for(int i = 0; i < pts.size(); i++){
							ofVertex(pts[i].x, pts[i].y);
						 } 
					ofEndShape();
				}
			}
			
			if( lastCircle ){
				ofFill();
				if( pts.size() ){
					ofCircle(pts.back().x, pts.back().y, 4);
				} 
			}
			ofPopStyle();
		}
		
		bool isCurve;
		
		ofRectangle rect;
		vector <strokePoint> pts;

};
