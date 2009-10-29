/*
 *  appUtils.h
 *  openFrameworks
 *
 *  Created by theo on 23/08/2009.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#pragma once 

#include "ofMain.h"

static string getTimeAsStr(){
	string sec		= "_" + ofToString( ofGetSeconds() );
	string min		= "_" + ofToString( ofGetMinutes() );
	string hour		= "_" + ofToString( ofGetHours() );
	string day		= "_" + ofToString( ofGetDay() );
	string month	= "_" + ofToString( ofGetMonth() );
	string year		= ofToString( ofGetYear() );
	
	return year+month+day+hour+min+sec;
}
