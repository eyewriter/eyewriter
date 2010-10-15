/*
 *  saverScene.h
 *  openFrameworks
 *
 *  Created by theo on 20/08/2009.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#pragma once

#include "ofMain.h"
#include "strokeGroup.h"
#include "buttonStateManager.h"
#include "buttonState.h"

#include "buttonRect.h"
#include "buttonToggle.h"
#include "buttonTrigger.h"
#include "strokeThumbnails.h"
#include "strokeUtils.h"
#include "ofxXmlSettings.h"
#include "grafIO.h"
#include "gmlUploader.h"
#include "gmlReader.h"

#include "appUtils.h"

#include "fileHelper.h"

#include "baseScene.h"

#include "simpleFtpUpload.h"
#include "ofxDirList.h"

class saverScene : public baseScene{

	public:

		void setup();
		void update(float mouseX, float mouseY);
		void draw();
		
		bool uploadImage(string localFolder, string imageName);

		void loadPreState();
		void loadNextState();
		void loadState();

		buttonState		testState;
		buttonTrigger   clearTags;
		
		buttonTrigger loadPrevious;
		buttonTrigger loadNext;
		
		buttonTrigger addTag;
		buttonTrigger clearLoaded;
		
		vector <strokeGroup> tmpGroups;

		bool grabScreen;

		gmlUploader uploader;
		gmlReader reader;		
		grafIO gml;
		
		string imageFilename;
		string imagePath;
		
		string gmlUploadString;
		string ftpDomainString;
		string ftpUser;
		string ftpPass;
		
		int ftpPort;
		
		simpleFtpUpload ftpUpload;
		
		buttonTrigger saveAndUpload;
		
		int whichState;
		ofxDirList dirList;
		
		int whichGroup;
		bool showAll;
		
		private:
			void 
			checkDirExists();
		
								
};

