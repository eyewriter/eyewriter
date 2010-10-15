/*
 *  styleScene.cpp
 *  openFrameworks
 *
 *  Created by keith on 20/08/2009.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#include "styleScene.h"
#include "renderManager.h"

//switch to gui
extern  float rate; 


//extern vector <strokeGroup>	groups;
#include "groupCollection.h"
extern groupCollection groups;

extern int buttonCount; 

renderManager renderer;


//--------------------------------------------------------------
void styleScene::setup(){
	
	//groups.push_back(strokeGroup());
	//groups.back().begin();
		
	ofBackground(255, 255, 255);
	
	//toggle drawing
	
	selectAll.setup("Select All", 10, 10, 130, 75);
	selectLetter.setup("Select Letter", 10, 110, 130, 75);
	strokeOptions.setup("Stroke Options", 10, 210, 130, 60);
	fillOptions.setup("Fill Options", 10, 295, 130, 60);
	shadowOptions.setup("Shadow Options", 10, 375, 130, 60);
	outlineOptions.setup("Outline Options", 10, 455, 130, 60);

	drawableButtons.push_back(&selectAll);
	drawableButtons.push_back(&selectLetter);
	drawableButtons.push_back(&strokeOptions);
	drawableButtons.push_back(&fillOptions);
	drawableButtons.push_back(&shadowOptions);
	drawableButtons.push_back(&outlineOptions);
	
	selectLeft.setup("< SELECT LETTER", 200, 20, 140, 100);
	selectLeft.setMaxCounter(buttonCount * 1.5);
	selectLeft.setRetrigger(true);

	selectRight.setup("SELECT LETTER >", 360, 20, 140, 100);
	selectRight.setMaxCounter(buttonCount * 1.5);
	selectRight.setRetrigger(true);	
	
	strokeDown.setup("< STROKE", 200, 20, 140, 100);
	strokeDown.setMaxCounter(buttonCount * 2);
	strokeDown.setRetrigger(true);
	
	strokeUp.setup("STROKE > ", 360, 20, 140, 100);
	strokeUp.setMaxCounter(buttonCount * 2);
	strokeUp.setRetrigger(true);
	
	brush.setup("BRUSH", 710, 20, 140, 100);
	brush.setMaxCounter(buttonCount * 2);
	brush.setRetrigger(true);
	
	toggleFill.setup("FILL","NO FILL", false, 200, 20, 140, 100);
	toggleFill.setMaxCounter(buttonCount * 2);
	
	toggleHoles.setup("HOLES","NO HOLES", false, 350, 20, 140, 100);
	toggleHoles.setMaxCounter(buttonCount * 2);
	
	lighting.setup("LIGHTING","NO LIGHTING", false, 710, 20, 140, 100);
	lighting.setMaxCounter(buttonCount * 2);
	
	hashes.setup("HASH","NO HASH", false, 560, 20, 140, 100);
	hashes.setMaxCounter(buttonCount * 2);

	shadowLeft.setup("< SHADOW", 200, 80, 110, 60);
	shadowLeft.setMaxCounter(buttonCount * 2);
	shadowLeft.setRetrigger(true);	
	
	shadowRight.setup("SHADOW >", 360, 80, 110, 60);
	shadowRight.setMaxCounter(buttonCount * 2);
	shadowRight.setRetrigger(true);	
	
	shadowUp.setup("^ SHADOW", 280, 10, 110, 60);
	shadowUp.setMaxCounter(buttonCount * 2);
	shadowUp.setRetrigger(true);	
	
	shadowDown.setup("v SHADOW", 280, 150, 110, 60);
	shadowDown.setMaxCounter(buttonCount * 2);
	shadowDown.setRetrigger(true);	
	
	
	outlineDown.setup("< OUTLINE", 200, 20, 140, 100);
	outlineDown.setMaxCounter(buttonCount * 2);
	outlineDown.setRetrigger(true);
	
	outlineUp.setup("OUTLINE > ", 360, 20, 140, 100);
	outlineUp.setMaxCounter(buttonCount * 2);
	outlineUp.setRetrigger(true);
	
	pickColor.setup("COLOR","COLOR", false, 860, 20, 140, 100);

	
	showAll		= true; drawColors = pickColor.getState();
	whichGroup	= 0;
	strokeWid = 1;

	cPicker.setup(0,0);
	
	mx = 0.0;
	my = 0.0;
	
	for(int i = 0; i < drawableButtons.size(); i++){
		((buttonTrigger *)drawableButtons[i])->setMaxCounter(buttonCount);
	}

	testState = BUTTON_NONE;
}

//--------------------------------------------------------------
void styleScene::update(float mouseX, float mouseY){

	if( whichGroup >= groups.size() ){
		whichGroup = 0;
	}

	
	selectRight.setMaxCounter(buttonCount * 2);
	selectLeft.setMaxCounter(buttonCount * 2);
	
	mx = mouseX;
	my = mouseY;

	if( selectAll.update(mx, my) ){
		state = STYLE_ALL;
		showAll = true;
		drawColors = false;
		pickColor.active = false;
	}else if( selectLetter.update(mx, my) ){
		state = STYLE_LETTER;
		showAll = false;
		drawColors = false;
		pickColor.active = false;
	}else if( strokeOptions.update(mx,my) ){
		state = STYLE_STROKE;
		drawColors = false;
		pickColor.active = false;
	}else if( fillOptions.update(mx,my) ){
		state = STYLE_FILL;
		drawColors = false;
		pickColor.active = false;
	}else if (shadowOptions.update(mx,my)){
		state = STYLE_SHADOW;
		drawColors = false;
		pickColor.active = false;
	}else if (outlineOptions.update(mx,my)){
		state = STYLE_OUTLINE;
		drawColors=false;
		pickColor.active = false;
	}
	
	switch(state){
		case(STYLE_LETTER ):
			if( selectLeft.update( mx , my ) ){
				whichGroup--;
				while ( !groups[ whichGroup ].hasPoints() ){
					whichGroup--;
					if( whichGroup < 0 ) whichGroup = groups.size() - 1;
				}
				if( whichGroup < 0 ) whichGroup = groups.size() - 1;
			}
			
			if( selectRight.update( mx, my ) ){
				whichGroup++;
				while ( !groups[ whichGroup ].hasPoints() ){
					whichGroup++;
					if( whichGroup >= groups.size() ) whichGroup = 0;
				}
				if( whichGroup >= groups.size() ) whichGroup = 0;
				
			}
		break;
			
		case(STYLE_STROKE):
			if (pickColor.update(mx,my)){
				drawColors = pickColor.getState();
			}
			
			if (brush.update(mx,my)){
				updateStyle("brushId", 1, 0, renderer.numBrushes-1, showAll, true);
			}
			
			if (strokeDown.update(mx,my)){
				updateStyle("strokeWeight", -1, 0, 10, showAll, false);
			}
			
			if (strokeUp.update(mx,my)){
				updateStyle("strokeWeight", 1, 0, 10, showAll, false);
			}
		break;
			
		case(STYLE_FILL):
			if (pickColor.update(mx,my)){
				drawColors = pickColor.getState();
			}
			
			if (!groups[whichGroup].style.doFill && toggleFill.getState()) toggleFill.active=false;
			if (groups[whichGroup].style.doFill && !toggleFill.getState()) toggleFill.active=true;
			
			
			
			if ( toggleFill.update(mx,my) ){
				if (showAll){
					for (int i=0;i<groups.size();i++){
						groups[i].style.doFill = toggleFill.getState();
					}
				}else groups[whichGroup].style.doFill = toggleFill.getState();
			}

			
			if (!groups[whichGroup].style.doHoles && toggleHoles.getState()) toggleHoles.active=false;
			if (groups[whichGroup].style.doHoles && !toggleHoles.getState()) toggleHoles.active=true;
			
			if (toggleHoles.update(mx,my)){
				if (showAll){
					for (int i=0;i<groups.size();i++){
						groups[i].style.doHoles = toggleHoles.getState();
					}
				}else groups[whichGroup].style.doHoles = toggleHoles.getState();
			}
		break;
			
		case(STYLE_SHADOW):
			
			if (pickColor.update(mx,my)){
				drawColors = pickColor.getState();
			}
			
			if (shadowLeft.update(mx,my)){
				updateStyle("dropShadowX",-3,-500,500,showAll,false);
			}
			
			if (shadowRight.update(mx,my)){
				updateStyle("dropShadowX",3,-500,500,showAll,false);
			}
			
			if (shadowUp.update(mx,my)){
				updateStyle("dropShadowY",-3,-500,500,showAll,false);
			}
			
			if (shadowDown.update(mx,my)){
				updateStyle("dropShadowY",3,-500,500,showAll,false);
			}
			
			if (!groups[whichGroup].style.dotShadow && lighting.getState()) lighting.active=false;
			if (groups[whichGroup].style.dotShadow && !lighting.getState()) lighting.active=true;
			
			if (lighting.update(mx,my)){
				if (showAll){
					for (int i=0;i<groups.size();i++){
						groups[i].style.dotShadow = lighting.getState();			
					}
				}else	groups[whichGroup].style.dotShadow = lighting.getState();			
			}
			
			if (!groups[whichGroup].style.shadowHash && hashes.getState()) hashes.active=false;
			if (groups[whichGroup].style.shadowHash && !hashes.getState()) hashes.active=true;
			
			if (hashes.update(mx,my)){
				if (showAll){
					for (int i=0;i<groups.size();i++){
						groups[i].style.shadowHash = hashes.getState();			
					}
				}else	groups[whichGroup].style.shadowHash = hashes.getState();			
			}
		
		break;
			
		case(STYLE_OUTLINE):
			
			if (pickColor.update(mx,my)){
				drawColors = pickColor.getState();
			}
			
			if (outlineDown.update(mx,my)){
				if (showAll){
					for (int i=0;i<groups.size();i++){
						groups[i].style.outline > 0 ? --groups[i].style.outline : groups[i].style.outline = 0;
					}
				}else	groups[whichGroup].style.outline > 0 ? --groups[whichGroup].style.outline : groups[whichGroup].style.outline = 0;
			}
			if (outlineUp.update(mx,my)){
				if (showAll){
					for (int i=0;i<groups.size();i++){
						groups[i].style.outline++;
					}
				}else	groups[whichGroup].style.outline++;
			}
		break;
		default: break;
	}
		
	
	
	// COLOR CHOICE
	if (drawColors) {
		int col;
		col = cPicker.update(mx,my);
		switch(state){
			case(STYLE_FILL):
				updateColor("fillColor",col,showAll);
			break;
			
			case(STYLE_STROKE):
				updateColor("strokeColor",col,showAll);
			break;
				
			case(STYLE_SHADOW):
				updateColor("shadowColor",col,showAll);
			break;
				
			case(STYLE_OUTLINE):
				updateColor("outlineColor",col,showAll);
			break;
				
			default: break;
		}
		
	}else{
		cPicker.outColor = -1;
	}
	
}


//--------------------------------------------------------------
void styleScene::draw(){
	ofPushStyle();	

	for(int i = 0; i < drawableButtons.size(); i++){
		drawableButtons[i]->draw();
	}
		
	if( groups.size() ){
		renderer.render(groups.getVector());
		ofPushStyle();
		ofSetLineWidth(1);
		glLineStipple(1, 0x3F07 ); 
		glEnable( GL_LINE_STIPPLE ); 
		for(int i = 0; i < groups.size(); i++){
			if( i == whichGroup || showAll){
				ofSetColor(160, 160, 160);
				groups[i].drawBoundingRect();
			}
		}
		glDisable(GL_LINE_STIPPLE);
		ofPopStyle();
	}
	
	ofEnableAlphaBlending();
	if( state == STYLE_LETTER ){
		selectLeft.draw(50.0);
		selectRight.draw(50.0);
	}else if (state == STYLE_STROKE){
		strokeUp.draw(50.0);
		strokeDown.draw(50.0);
		pickColor.draw(50.0);
		brush.draw(50.0);
	}else if (state == STYLE_SHADOW){
		shadowLeft.draw(50.0);
		shadowRight.draw(50.0);
		shadowUp.draw(50.0);
		shadowDown.draw(50.0);
		pickColor.draw(50.0);
		lighting.draw(50.0);
		hashes.draw(50.0);
	}else if (state == STYLE_FILL){
		toggleFill.draw(75.0);
		toggleHoles.draw(75.0);
		pickColor.draw(50.0);
	}else if (state == STYLE_OUTLINE){
		outlineUp.draw(50.0);
		outlineDown.draw(50.0);
		pickColor.draw(50.0);
	}
	
	if (drawColors) {
		ofPushStyle();
		cPicker.draw(pickColor.x + pickColor.width - 6*cPicker.width, pickColor.y+100);
		ofPopStyle();
	}
	
	
	if( mx < SIDE_GUI_X ){
		ofPushStyle();
		ofFill();
		ofSetColor(255, 255, 255);
		ofCircle(mx, my, 9);
		ofSetColor(0,0,0);
		ofCircle(mx, my, 6);
		ofPopStyle();
	}
	else if( testState == BUTTON_NONE ){
		ofSetColor(0, 0, 0);
		ofNoFill();
		ofCircle(mx, my, 9);
	}else if(testState == BUTTON_STARTED ){
		ofFill();
		ofCircle(mx, my, 5);
	}
	
	ofPopStyle();
}


void styleScene::updateStyle(string ss, int val, int min, int max, bool all, bool cycle){
	if (all){
		for ( int i = 0; i < groups.size(); i++ ){
			int out = groups[i].style.atts[ss]+val;
			if (cycle) {
				if ( out > max) out = min; else if (out < min) out = max;
			} else out = ofClamp(out, min, max);
			groups[i].style.atts[ss] = out;
//			cout << groups[i].style.atts[ss] << endl;
		}
	}else{
		int out = groups[whichGroup].style.atts[ss]+val;
		if (cycle) {
			if ( out > max) out = min; else if (out < min) out = max;
		} else out = ofClamp(out, min, max);
		groups[whichGroup].style.atts[ss] = out;
//		cout << groups[whichGroup].style.atts[ss] << endl;
	}
}

void styleScene::updateColor(string ss, int c, bool all){
	if( c != -1 ){ 
		if ( all){
			for ( int i = 0; i < groups.size(); i++ ){
				groups[i].style.colors[ss] = c;
			}
		}else{
			groups[whichGroup].style.colors[ss] = c;
		}
	}
	
}


