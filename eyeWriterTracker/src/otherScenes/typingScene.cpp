/*
 *  typingScene.cpp
 *  openFrameworks
 *
 *  Created by andrea bradshaw on 4/10/2010.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#include "typingScene.h"

//switch to gui
extern  int buttonCount; 

//--------------------------------------------------------------
void typingScene::setup(){
	
	franklinBook.loadFont("fonts/HelveticaNeueMed.ttf", 32);
	
	franklinBookSmall.loadFont("fonts/HelveticaNeueMed.ttf", 16);
	carriageReturnCounter = 0;
	//buttonCount=390;
	ofBackground(255, 255, 255);
	shiftOn = false;
	
	string buttons[36] = 
	{"!\n1", "@\n2", "#\n3", "$\n4", "%\n5", "^\n6", "&\n7", "*\n8", "(\n9", ")\n0", 
		"A", "B", "C", "D", "E", "F", "G", "H", "I", "J", 
		"K", "L", "M", "N", "O", "P", "Q", "R", "S", 
		"T", "U", "V", "W", "X", "Y", "Z"
	};
	
	bSpeakWords = false;
	
	float xadd2 = 0;
	float yadd2 = 200;
	
	
	float xStart  = 15;
	float yStart  = 15;
	float bWidth  = 100;
	float bHeight = 100;
	
	for (int i = 0; i < 36; i++){
		buttonTrigger nButton;
		nButton.setup(buttons[i], xStart + xadd2, yStart + yadd2, bWidth, bHeight);
		nButton.setMaxCounter(buttonCount);
		nButton.setRetrigger(false);
		letterButtons.push_back(nButton);
		
		xStart += 120;
		if (xStart > 1100){
			//if (xStart > 900){
			xStart = 90;
			yStart += 120;
		}
		
	}
	
	float xadd = 150;
	float yadd = 250;
	
	bWidth  = 75;
	bHeight = 75;
	
	xadd = 350;
	
	buttonTrigger semiButton;
	semiButton.setup(":\n;", 825+xadd, 295+yadd, bWidth, bHeight);
	semiButton.setMaxCounter(buttonCount);
	semiButton.setRetrigger(false);
	letterButtons.push_back(semiButton);
	
	buttonTrigger quoteButton;
	quoteButton.setup("\"\n'", 915+xadd, 295+yadd, bWidth, bHeight);
	quoteButton.setMaxCounter(buttonCount);
	quoteButton.setRetrigger(false);
	letterButtons.push_back(quoteButton);
	
	buttonTrigger commaButton;
	commaButton.setup("<\n,", 735+xadd, 385+yadd, bWidth, bHeight);
	commaButton.setMaxCounter(buttonCount);
	commaButton.setRetrigger(false);
	letterButtons.push_back(commaButton);
	
	buttonTrigger periodButton;
	periodButton.setup(">\n.", 825+xadd, 385+yadd, bWidth, bHeight);
	periodButton.setMaxCounter(buttonCount);
	periodButton.setRetrigger(false);
	letterButtons.push_back(periodButton);
	
	buttonTrigger questionButton;
	questionButton.setup("?\n/", 915+xadd, 385+yadd, bWidth, bHeight);
	questionButton.setMaxCounter(buttonCount);
	questionButton.setRetrigger(false);
	letterButtons.push_back(questionButton);
	
	//buttonTrigger deleteButton;
//	deleteButton.setup("ENTER", 735+xadd, 475+yadd, bWidth*2+15, bHeight);
//	deleteButton.setMaxCounter(buttonCount);
//	deleteButton.setRetrigger(true);
//	letterButtons.push_back(deleteButton);
	
	
	buttonTrigger enterButton;
	enterButton.setup("DELETE", 195+xadd, 475+yadd, bWidth*2+15, bHeight);
	enterButton.setMaxCounter(buttonCount);
	enterButton.setRetrigger(true);
	letterButtons.push_back(enterButton);
	
	buttonTrigger speakAllButton;
	speakAllButton.setup("SPEAK", 15+xadd - 200, 475+yadd, bWidth*2+15, bHeight);
	speakAllButton.setMaxCounter(buttonCount);
	speakAllButton.setRetrigger(false);
	letterButtons.push_back(speakAllButton);
	
	
	buttonTrigger clearButton;
	clearButton.setup("CLEAR ALL", 15+xadd, 475+yadd, bWidth*2+15, bHeight);
	clearButton.setMaxCounter(buttonCount);
	clearButton.setRetrigger(false);
	letterButtons.push_back(clearButton);
	
	buttonTrigger spaceButton;
	spaceButton.setup("SPACE", 375+xadd, 475+yadd, bWidth*4.5+7, bHeight);
	spaceButton.setMaxCounter(buttonCount);
	spaceButton.setRetrigger(false);
	letterButtons.push_back(spaceButton);
	
	buttonToggle capsButton;
	capsButton.setup("CAPS ON", "CAPS OFF", false, 800+xadd+100, 475+yadd, bWidth+15, bHeight);
	capsButton.setMaxCounter(buttonCount);
	actionButtons.push_back(capsButton);
	
	
	buttonToggle speakButton;
	speakButton.setup("SPEAK\nWORDS\nON", "SPEAK\nWORDS\nOFF", false, 800+xadd+100, 175+yadd, bWidth+15, bHeight);
	speakButton.setMaxCounter(buttonCount);
	actionButtons.push_back(speakButton);
	
	
	//
	letterButtons_lower.push_back("1");
	letterButtons_lower.push_back("2");
	letterButtons_lower.push_back("3");
	letterButtons_lower.push_back("4");
	letterButtons_lower.push_back("5");
	letterButtons_lower.push_back("6");
	letterButtons_lower.push_back("7");
	letterButtons_lower.push_back("8");
	letterButtons_lower.push_back("9");
	letterButtons_lower.push_back("0");
	
	letterButtons_lower.push_back("a");
	letterButtons_lower.push_back("b");
	letterButtons_lower.push_back("c");
	letterButtons_lower.push_back("d");
	letterButtons_lower.push_back("e");
	letterButtons_lower.push_back("f");
	letterButtons_lower.push_back("g");
	letterButtons_lower.push_back("h");
	letterButtons_lower.push_back("i");
	letterButtons_lower.push_back("j");
	
	letterButtons_lower.push_back("k");
	letterButtons_lower.push_back("l");
	
	letterButtons_lower.push_back("m");
	
	letterButtons_lower.push_back("n");
	letterButtons_lower.push_back("o");
	letterButtons_lower.push_back("p");
	letterButtons_lower.push_back("q");
	letterButtons_lower.push_back("r");
	letterButtons_lower.push_back("s");
	
	letterButtons_lower.push_back("t");
	letterButtons_lower.push_back("u");
	letterButtons_lower.push_back("v");
	letterButtons_lower.push_back("w");
	letterButtons_lower.push_back("x");
	letterButtons_lower.push_back("y");
	letterButtons_lower.push_back("z");
	letterButtons_lower.push_back(";");
	
	letterButtons_lower.push_back("'");
	letterButtons_lower.push_back(",");
	letterButtons_lower.push_back(".");
	letterButtons_lower.push_back("/");
	
	
	for (int i = 0; i < letterButtons.size(); i++){
		
		letterButtons[i].setDisplayFont(&franklinBookSmall);
	}
	
	mx = 0.0;
	my = 0.0; 
	
	
	
	
}
//--------------------------------------------------------------
void typingScene::update(float mouseX, float mouseY){
	mx = mouseX;
	my = mouseY;
	
	//ofSetFrameRate(100);
	
	for(int i = 0; i < letterButtons.size(); i++) {
		letterButtons[i].setMaxCounter(buttonCount);
		if(letterButtons[i].update(mx, my)) {
			if ((carriageReturnCounter == 32)|(carriageReturnCounter == 31)) {
				//displayMessage.push_back("\n");
				//carriageReturnCounter = 0;
			}
			//else if (letterButtons[i].displayText == "ENTER"){
//				displayMessage.push_back("\n");
//				carriageReturnCounter = 0;
//			}
			else if (letterButtons[i].displayText == "SPACE"){
				
				
				if (bSpeakWords == true){
				vector <string> wordsToSpeak = ofSplitString(displayMessage, " ");
				
				if (wordsToSpeak.size() > 0 && displayMessage.size() >= 1){
					if (displayMessage[displayMessage.size()-1] != ' '){
						
					if (wordsToSpeak[wordsToSpeak.size()-1].size() > 0){
						speakMe(	wordsToSpeak[wordsToSpeak.size()-1].c_str() );
					}
					}
				}
				}
				displayMessage.push_back(' ');
				carriageReturnCounter++;
			}
			else if (letterButtons[i].displayText == "CLEAR ALL"){
				displayMessage.clear();
				carriageReturnCounter = 0;
			}
			
			else if (letterButtons[i].displayText == "SPEAK"){
				if (displayMessage.size() > 0){
					speakMe(displayMessage.c_str());
				}
			}
			
			else if (letterButtons[i].displayText == "DELETE"){
				if (displayMessage.size()> 0){
					displayMessage.resize (displayMessage.size () - 1);
				}
			}			
			else if (shiftOn && (letterButtons[i].displayText == "!\n1")){
				displayMessage.push_back('!');
				carriageReturnCounter++;
			}
			else if (shiftOn && (letterButtons[i].displayText == "@\n2")){
				displayMessage.push_back('@');
				carriageReturnCounter++;
			}
			else if (shiftOn && (letterButtons[i].displayText == "#\n3")){
				displayMessage.push_back('#');
				carriageReturnCounter++;
			}
			else if (shiftOn && (letterButtons[i].displayText == "$\n4")){
				displayMessage.push_back('$');
				carriageReturnCounter++;
			}
			else if (shiftOn && (letterButtons[i].displayText == "%\n5")){
				displayMessage.push_back('%');
				carriageReturnCounter++;
			}
			else if (shiftOn && (letterButtons[i].displayText == "^\n6")){
				displayMessage.push_back('^');
				carriageReturnCounter++;
			}
			else if (shiftOn && (letterButtons[i].displayText == "&\n7")){
				displayMessage.push_back('&');
				carriageReturnCounter++;
			}
			else if (shiftOn && (letterButtons[i].displayText == "*\n8")){
				displayMessage.push_back('*');
				carriageReturnCounter++;
			}
			else if (shiftOn && (letterButtons[i].displayText == "(\n9")){
				displayMessage.push_back('(');
				carriageReturnCounter++;
			}
			else if (shiftOn && (letterButtons[i].displayText == ")\n0")){
				displayMessage.push_back(')');
				carriageReturnCounter++;
			}
			else if (shiftOn && (letterButtons[i].displayText == ":\n;")){
				displayMessage.push_back(':');
				carriageReturnCounter++;
			}
			else if (shiftOn && (letterButtons[i].displayText == "\"\n'")){
				displayMessage.push_back('\"');
				carriageReturnCounter++;
			}
			else if (shiftOn && (letterButtons[i].displayText == "<\n,")){
				displayMessage.push_back('<');
				carriageReturnCounter++;
			}
			else if (shiftOn && (letterButtons[i].displayText == ">\n.")){
				displayMessage.push_back('>');
				carriageReturnCounter++;
			}
			else if (shiftOn && (letterButtons[i].displayText == "?\n/")){
				displayMessage.push_back('?');
				carriageReturnCounter++;
			}
			else {
				if (shiftOn){
					displayMessage.push_back(letterButtons[i].displayText.c_str()[0]);
					//carriageReturnCounter += 2;
				}
				else {
					displayMessage.push_back(letterButtons_lower[i].c_str()[0]);
					//carriageReturnCounter++;
				}
			}
		}
		
	}
	
	for (int i = 0; i < actionButtons.size(); i++){
		actionButtons[i].setMaxCounter(buttonCount);
		if(actionButtons[i].update(mx, my)) {
			if (actionButtons[i].displayText[1] == "CAPS ON"){
				if (shiftOn) shiftOn = false;
				else shiftOn = true;
			}
		}
	}
	
	
	bSpeakWords = false;
	for (int i = 0; i < actionButtons.size(); i++){
		if (actionButtons[i].displayText[1] == "SPEAK\nWORDS\nON"){
			if (actionButtons[i].active){
				bSpeakWords = true;	
			}
		}
	}
	
	//displayMessage = "";
	//for (int i = 0; i < displayMessage.size(); i++){
//		displayMessage += message[i];
//	}
	
}	

//--------------------------------------------------------------
void typingScene::draw(){
	ofPushStyle();	
	
	//bool bGrid = showGrid.getState();
	
	for(int i = 0; i < letterButtons.size(); i++){
		letterButtons[i].draw();
	}
	for(int i = 0; i < actionButtons.size(); i++){
		actionButtons[i].draw();
	}
	
	//float textWidth = 8.0f * displayMessage.length();
	//float remainX = (width - textWidth)/2;
	
	//float textHeight = 14.0f;
	//float remainY = (height - textHeight)/2.0f + (textHeight/2.0f);
	ofFill();
	ofSetColor(0,0,0);
	ofRect(0,0,ofGetWidth(), 200);
	
	
	ofSetColor(255,255,255);
	//ofDrawBitmapString(displayMessage, 200, 600);
	
	
	string layedoutmessage = "";
	
	if (displayMessage.size() > 0){
		int count = 0;
		for (int i = 0; i < displayMessage.size(); i++){
			layedoutmessage.push_back(displayMessage[i]);
			count++;
			if (count >  58){
				layedoutmessage += "-\n";
				count = 0;
			}
		}
	}
	
	franklinBook.drawString(layedoutmessage, 30, 50);
	
	
	vector <string> strings = ofSplitString(layedoutmessage, "\n");
	string subMessage = layedoutmessage;
	if(strings.size() > 1){
		subMessage = strings[strings.size()-1];
	}
	float xx = franklinBook.getStringBoundingBox(subMessage + ".", 30, 50).x + franklinBook.getStringBoundingBox(subMessage+ ".", 30, 50).width;
	float yy = franklinBook.getStringBoundingBox(layedoutmessage + ".", 30, 50).y + franklinBook.getStringBoundingBox(layedoutmessage+ ".", 30, 50).height;
	
	ofSetColor(127, 127, 127);
	ofRect(xx, yy, 10,3);
	
	ofPopStyle();
	
	//drawCursor();
}


