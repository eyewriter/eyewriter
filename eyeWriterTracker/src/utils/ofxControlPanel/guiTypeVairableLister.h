/*
 *  guiTypeVairableLister.h
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

class guiTypeVairableLister : public guiBaseObject{

    public:

        guiTypeVairableLister(){
			typeStartOffset = 0;
		}

        void setup(string listerName, vector <guiVariablePointer> & varsIn){
			name = listerName;
			vars = varsIn;
		}
		
		void update(){			
			updateBoundingBox();
		}
		
		void updateBoundingBox(){
		
			minNameWidth = 20;
			minVarWidth  = 20;
			
			if( displayText.usingTTF() ){
				typeStartOffset = -4;
			}else{
				typeStartOffset = -2;
			}
							
			for(int i = 0; i < vars.size(); i++){
				minNameWidth = MAX(minNameWidth, displayText.getTextWidth(vars[i].displayName));
				
				if( vars[i].dataType == GUI_VAR_FLOAT ){
					vars[i].varAsString = ofToString( *((float *)vars[i].ptr), vars[i].precision);
				}else if( vars[i].dataType == GUI_VAR_INT ){
					vars[i].varAsString = ofToString( *((int *)vars[i].ptr));
				}else if( vars[i].dataType == GUI_VAR_STRING ){
					vars[i].varAsString = *((string *)vars[i].ptr);
				}
				
				minVarWidth = MAX(minVarWidth, displayText.getTextWidth( "= " + vars[i].varAsString));
				
			}
			
			minNameWidth += 5;			
			minVarWidth  += 5;
			
			hitArea.width  = MAX(hitArea.width, minNameWidth + minVarWidth);
			hitArea.height = MAX(20, vars.size() * ( displayText.getTextSingleLineHeight() + 3));
			
			if(bShowText){
				//we need to update out hitArea because the text will have moved the gui down
				hitArea.y = boundingBox.y + displayText.getTextHeight() + titleSpacing;
				boundingBox.height = hitArea.height + displayText.getTextHeight() + titleSpacing;
			}else{
				 //we need to update out hitArea because the text will have moved the gui down
				hitArea.y = boundingBox.y;
			}
		}
		
		//-----------------------------------------------.
		void render(){
			ofPushStyle();
				glPushMatrix();
				
					glColor4fv(textColor.getColorF());				
					guiBaseObject::renderText();

					//draw the background
					ofFill();
					glColor4fv(bgColor.getColorF());
					ofRect(hitArea.x, hitArea.y, hitArea.width, hitArea.height);

					float lineH = hitArea.height / (float) MAX(1, vars.size());
					
					float x		= hitArea.x;
					float y		= (hitArea.y + lineH) + typeStartOffset;
					float ly	= (hitArea.y + lineH);
					
					//draw the foreground
					for(int i = 0; i < vars.size(); i++){
						
						if( i != vars.size()-1){
							glColor4fv(outlineColor.getColorF());
							ofLine(x, ly, x + hitArea.width, ly);
						}
						
						glColor4fv(textColor.getColorF());
						displayText.renderString(vars[i].displayName, x, y);
						displayText.renderString("= " + vars[i].varAsString, x + minNameWidth, y);

						y  += lineH;
						ly += lineH;
					}
					
					//draw the outline
					ofNoFill();
					glColor4fv(outlineColor.getColorF());
					ofRect(hitArea.x, hitArea.y, hitArea.width, hitArea.height);
				glPopMatrix();
			ofPopStyle();
		}
				
		vector <guiVariablePointer> vars;
		float minNameWidth, minVarWidth;
		float typeStartOffset;
		
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
