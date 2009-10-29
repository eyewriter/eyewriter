/*
 *  buttonStateManager.h
 *  openFrameworks
 *
 *  Created by theo on 17/08/2009.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#pragma once

#include "ofMain.h"
#include "buttonState.h"
#include "ofxVec2f.h"

class timePoint : public ofxVec2f{
	public:
		timePoint(float _x, float _y, float _t){
			x = _x;
			y = _y;
			t = _t;
		}
		float t;
};


class buttonStateManager{

	public:
		buttonStateManager(){
			state = BUTTON_NONE;
			waitTime = 0.9f;
			needsMove = false;
		}
		
		void setWaitTime(float newWaitTime){
			waitTime = newWaitTime;
		}	
		
		void clear(){
			ptHistory.clear();
		}

		void update(float x, float y){
		
			ptHistory.push_back(timePoint(x, y, ofGetElapsedTimef()));
			
			float currentT = ofGetElapsedTimef();
			
			for(int i = 0; i < ptHistory.size(); i++){
				int k = ptHistory.size() - (i+1);
				
				if( currentT - ptHistory[k].t > waitTime ){
					ptHistory.erase(ptHistory.begin(), ptHistory.begin() + k);
					break;	
				 } 
			}
			
		}
		
		bool isPointStationary(float radiusThreshold ){
			
			float threshToCheck = radiusThreshold;
			
			bool isStationary	= false;
			
			if( ptHistory.size() ){
				
				ofxVec2f averagePos = 0.0f;
				
				float oldestPoint = ptHistory[0].t;
				if( ofGetElapsedTimef() - oldestPoint <= waitTime){
					return false;
				}
				
				for(int i = 0; i < ptHistory.size(); i++){
					averagePos += ptHistory[i];
				}
				averagePos /= ptHistory.size();

				isStationary = true;
				for(int i = 0; i < ptHistory.size(); i++){
					if( ( ptHistory[i] - averagePos ).lengthSquared() > threshToCheck*threshToCheck ){
						isStationary = false;
						break;
					}
				}
				
				if( isStationary && needsMove == false){
					ptHistory.clear();
					needsMove = true;
				}else if( isStationary ){
					isStationary = false;
				}else{
					needsMove = false;
				}

			}
			
			return isStationary;
		}

		void setStartThresh(float startThresh){
			
		}

		void setEndThresh(float endThresh){
		
		}
		
		float startThresh;
		float endThresh;

		float waitTime;

		vector <timePoint> ptHistory;

		buttonState state;
		bool needsMove;
};