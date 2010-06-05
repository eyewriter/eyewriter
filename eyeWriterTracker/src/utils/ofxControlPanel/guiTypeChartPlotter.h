/*
 *  guiTypeChartPlotter.h
 *  ofxControlPanelDemo
 *
 *  Created by theo on 01/04/2010.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#pragma once

#include "guiBaseObject.h"
#include "guiColor.h"
#include "simpleColor.h"
#include "guiValue.h"
#include "guiVariablePointer.h"

class guiStatVarPointer : public guiVariablePointer{
	public:
		guiStatVarPointer(){
			displayName = "none";
			ptr			= NULL;
			dataType	= GUI_VAR_FLOAT;		
			precision   = 7;
			autoUpdate  = true;		
		}

		guiStatVarPointer( string displayNameIn, void * varPtr, guiVarType theDataType, bool autoUpdateGraph, float floatPrecision = 7){
			displayName = displayNameIn;
			ptr			= varPtr;
			dataType	= theDataType;		
			precision   = floatPrecision;
			autoUpdate  = autoUpdateGraph;
		} 
	
		void setup( string displayNameIn, void * varPtr, guiVarType theDataType, bool autoUpdateGraph, float floatPrecision = 7){
			displayName = displayNameIn;
			ptr			= varPtr;
			dataType	= theDataType;		
			precision   = floatPrecision;
			autoUpdate  = autoUpdateGraph;
		} 
		
		void updateManually(){
			bDoUpdate = true;
		}

		bool autoUpdate;
		bool bDoUpdate;
};

class guiTypeChartPlotter : public guiBaseObject{

    public:

        guiTypeChartPlotter(){
			minVal = 0.0;
			maxVal = 1.0;
			maxNum = 100;
		}

        void setup(string elementName, guiStatVarPointer varPtr, float width, float height, int maxValues, float minValY, float maxValY){
			name		   = elementName;
			
			hitArea.width  = width;
			hitArea.height = height;
			
			minVal         = minValY;
			maxVal		   = maxValY;
			
			var			   = varPtr;
			maxNum		   = maxValues;
		}
		
		//-----------------------------------------------
		void update(){			
			updateBoundingBox();
			
			if( var.ptr == NULL ){
				ofLog(OF_LOG_ERROR, "guiTypeChartPlotter::update - variable ptr is not set!");
			}
			
			if( var.dataType == GUI_VAR_FLOAT ){
				var.varAsString = ofToString( *((float *)var.ptr), var.precision);
			}else if( var.dataType == GUI_VAR_INT ){
				var.varAsString = ofToString( *((int *)var.ptr));
			}
			
			if( var.autoUpdate || var.bDoUpdate ){
			
				valueHistory.push_back( *( (float *)var.ptr) );
				if( valueHistory.size() > maxNum ){
					valueHistory.erase(valueHistory.begin(), valueHistory.begin()+1);
				}
				
				var.bDoUpdate = false;
			}
			
		}
		
		//-----------------------------------------------
		void render(){
			ofPushStyle();
				glPushMatrix();
				
					glColor4fv(textColor.getColorF());				
					guiBaseObject::renderText();

					//draw the background
					ofFill();
					glColor4fv(bgColor.getColorF());
					ofRect(hitArea.x, hitArea.y, hitArea.width, hitArea.height);
					
					float x = hitArea.x;
					float y = hitArea.y + hitArea.height;
					
					if( valueHistory.size() ){
						glColor4fv(fgColor.getSelectedColorF());
					
						ofNoFill();
						ofBeginShape();
						for(int i = 0; i < valueHistory.size(); i++){
							float xx = ofMap(i, 0, maxNum, x, x + hitArea.width, true);
							float yy = ofMap(valueHistory[i], minVal, maxVal, y, y - hitArea.height, true);
							
							ofVertex(xx, yy);
						}
						ofEndShape(false);
					}
					
					ofFill();

//					float lineH = hitArea.height / (float) MAX(1, vars.size());
//					
//					float x		= hitArea.x;
//					float y		= (hitArea.y + lineH) + typeStartOffset;
//					float ly	= (hitArea.y + lineH);
//					
//					//draw the foreground
//					for(int i = 0; i < vars.size(); i++){
//						
//						if( i != vars.size()-1){
//							glColor4fv(outlineColor.getColorF());
//							ofLine(x, ly, x + hitArea.width, ly);
//						}
//						
//						glColor4fv(textColor.getColorF());
//						displayText.renderString(vars[i].displayName, x, y);
//						displayText.renderString("= " + vars[i].varAsString, x + minNameWidth, y);
//
//						y  += lineH;
//						ly += lineH;
//					}

					glColor4fv(textColor.getColorF());				
					displayText.renderString(var.displayName + ": " + var.varAsString, x + 2, y - displayText.getTextSingleLineHeight()*3);
					displayText.renderString("max: "+ofToString(maxVal, 0), x + 2, hitArea.y + displayText.getTextSingleLineHeight()+2);
					displayText.renderString("min: "+ofToString(minVal, 0), x + 2, y - 8);
					
					//draw the outline
					ofNoFill();
					glColor4fv(outlineColor.getColorF());
					ofRect(hitArea.x, hitArea.y, hitArea.width, hitArea.height);
				glPopMatrix();
			ofPopStyle();
		}
		
		float minVal, maxVal;
		vector <float> valueHistory;
		int maxNum;
		guiStatVarPointer var;
		
		
//        
//		void updateGui(float x, float y, bool firstHit, bool isRelative);
//        void setKnobSize(float _knobSize);
//        
//		virtual void setValue(float _value, int whichParam);
//        virtual void updateValue();
//		
//		virtual void notify();	
//		
//        void render();

};
