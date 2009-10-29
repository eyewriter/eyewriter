/*
 *  saverScene.cpp
 *  openFrameworks
 *
 *  Created by theo on 23/08/2009.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#include "saverScene.h"
#include "renderManager.h"

//switch to gui
extern  float rate; 

//extern vector <strokeGroup>	groups;
#include "groupCollection.h"
extern groupCollection groups;

extern int buttonCount; 

extern renderManager renderer;

//LEAVE THIS AS FALSE - THEO ONLY ONE WHO TESTS UPLOAD
static bool bUpload = false;

//--------------------------------------------------------------
void saverScene::setup(){
		
	ofBackground(255, 255, 255);
	
	saveAndUpload.setup("save tag", 10, 10, 130, 100);	
	saveAndUpload.setMaxCounter(buttonCount);
	
	saveAndUpload.setFlashLength(4.5);
	
	clearTags.setup("delete all tags", 10, 190, 130, 50);	
	clearTags.setMaxCounter(buttonCount*2);	
	
	addTag.setup("add tag", 650, 20, 140, 100);
	addTag.setMaxCounter(buttonCount*2);
	
	clearLoaded.setup("clear loaded tags", 840, 20, 140, 100);

	ofxXmlSettings xml;
	xml.loadFile("settings.xml");
	
	loadPrevious.setup("load prev tag", 10, 300, 130, 80);
	loadPrevious.setRetrigger(true);
	loadNext.setup("load next tag", 10, 400, 130, 80);	
	loadNext.setRetrigger(true);
	
	//move to xml
	gmlUploadString = "????????????";
	ftpDomainString = xml.getValue("FTP:HOST", "fffff.at");
	ftpUser			= xml.getValue("FTP:USER", "?????????");
	ftpPass			= xml.getValue("FTP:PASS", "?????????");
	ftpPort			= xml.getValue("FTP:PORT", 21);
	
	ftpUpload.setup(ftpDomainString, ftpUser, ftpPass, ftpPort);
				
	imageFilename = "";
	
	grabScreen = false;

	whichState = 0;

	mx = 0.0;
	my = 0.0;

}

void saverScene::loadPreState(){
	whichState--;
	loadState();
}

void saverScene::loadNextState(){
	whichState++;
	loadState();
}

void saverScene::loadState(){
	int numFiles = dirList.listDir("tags/gml");
	
	if( whichState < 0 )whichState = numFiles-1;
	if( whichState >= numFiles)whichState = 0;
	
	
	if( numFiles == 0 ){
		whichState = 0; 
		return;
	}
	
	gml.loadState(tmpGroups, dirList.getPath(whichState));
}

void saverScene::checkDirExists(){
	
	if( !fileHelper::doesDirectoryExist("tags") ){
		fileHelper::makeDirectory("tags/");
		fileHelper::makeDirectory("tags/images/");
		fileHelper::makeDirectory("tags/gml/");
	}
	
	if( !fileHelper::doesDirectoryExist("tags/images/") ){
		fileHelper::makeDirectory("tags/images/");
	}
			
	if( !fileHelper::doesDirectoryExist("tags/gml/") ){
		fileHelper::makeDirectory("tags/gml/");
	}
		

}

bool saverScene::uploadImage(string localFolder, string imageName){

	bool bUploaded = false;

	string remoteFolder   = "eyetags/";

	int numBytesUploaded = ftpUpload.uploadFile(imageName, localFolder, remoteFolder);

	if( numBytesUploaded <= 0 ){
		bUploaded = true;
	}else{
		bUploaded = false;
	}
	
	return bUploaded;

}

//--------------------------------------------------------------
void saverScene::update(float mouseX, float mouseY){

	saveAndUpload.setMaxCounter(buttonCount);
	clearTags.setMaxCounter(buttonCount*2);	
	
	mx = mouseX;
	my = mouseY;

	if( saveAndUpload.update(mx, my) ){
		
		checkDirExists();
		
		string timeStr = getTimeAsStr();
		
		string fileName = "temptTag-"+timeStr+".gml";
		
		string fullPath = "tags/gml/"+fileName;
		
		imagePath	    = "tags/images/";
		imageFilename   = "temptTag-"+timeStr+".png";
		
		//add the file name to gml
		gml.saveStateAndFileName(groups.getVector(), fullPath, fileName);
				
		reader.loadGML(fullPath);
		
		if( bUpload )uploader.uploadToWeb(gmlUploadString, reader.xml);
		if( bUpload )grabScreen = true;
	}
	
	if( tmpGroups.size() ){
		if( addTag.update(mx, my) ){
			vector <strokeGroup> & vec = groups.getVector();
			for(int i = 0; i < tmpGroups.size(); i++){
				vec.push_back(tmpGroups[i]);
			}
			tmpGroups.clear();
		}
		if( clearLoaded.update(mx, my) ){
			tmpGroups.clear();
		}
	}
	
	if( loadNext.update(mx, my) ){
		loadNextState();
	}
	
	if( loadPrevious.update(mx, my) ){
		loadPreState();
	}
		
	if( clearTags.update(mx, my) ){
		groups.clear();
		groups.addGroup();	
	}
	
}


//--------------------------------------------------------------
void saverScene::draw(){
	ofPushStyle();	

	ofFill();
	
	ofSetColor(0xFFFFFF);
	ofRect(SIDE_GUI_X, 0, ofGetWidth(), ofGetHeight());
	
	if( tmpGroups.size() ){
		ofPushStyle();
		ofEnableAlphaBlending();
		addTag.draw(50);
		clearLoaded.draw(50);
		ofPopStyle();
		
	}
	

	renderer.render(groups.getVector());
	renderer.render(tmpGroups);
	
	
	if( grabScreen ){
		ofImage img;
		img.grabScreen(SIDE_GUI_X, 0, ofGetWidth()-SIDE_GUI_X, ofGetHeight());
		img.saveImage(imagePath + imageFilename);
		
		if( !uploadImage(imagePath, imageFilename) ){
			printf("ERROR UPLOADING VIA FTP\n");
		}		
		
		grabScreen = false;
	}

	saveAndUpload.draw();
	clearTags.draw();
	loadPrevious.draw();
	loadNext.draw();
	
			
	if( mx < SIDE_GUI_X ){
		ofPushStyle();
			ofFill();
			ofSetColor(255, 255, 255);
			ofCircle(mx, my, 9);
			ofSetColor(0,0,0);
			ofCircle(mx, my, 6);
		ofPopStyle();
	}
	else{
		ofNoFill();
		ofCircle(mx, my, 9);
	}
	
	
	ofPopStyle();
}


