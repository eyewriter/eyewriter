#include "simpleLogger.h"

simpleLogger::simpleLogger(){
	fileLoaded = false;
	bDate = true;
	bTime = true;
	bLevel = true;
}

//------------------------------------------------
simpleLogger::~simpleLogger(){
	if(fileLoaded){
		saveFile();
	}
}

//------------------------------------------------
void simpleLogger::setup(string logFileName, bool overwrite){
	logFile = logFileName;

	fileLoaded = xml.loadFile(logFile);
	if(overwrite){
		xml.clear();
		xml.saveFile(logFile);
	}

	if( !fileLoaded ){
		xml.saveFile(logFile);
		fileLoaded = true;
	}
}

void simpleLogger::setIncludeDate(bool bIncludeDate){
	bDate = bIncludeDate;
}

void simpleLogger::setIncludeTime(bool bIncludeTime){
	bTime = bIncludeTime;
}

void simpleLogger::setIncludeLevelbool(bool  bIncludeLevel){
	bLevel = bIncludeLevel;
}

//--------------------------------------------------
void simpleLogger::log(int logLevel, const char* format, ...){
	//thanks stefan!
	//http://www.ozzu.com/cpp-tutorials/tutorial-writing-custom-printf-wrapper-function-t89166.html

	logRecord record;
	record.year     = ofGetYear();
	record.month    = ofGetMonth();
	record.day      = ofGetDay();
	record.hour     = ofGetHours();
	record.minute   = ofGetMinutes();
	record.seconds  = ofGetSeconds();
	record.level    = logLevel;

	char str[2048] = {0};

	va_list args;
	va_start( args, format );
	vsprintf(str, format, args );
	va_end( args );

	record.msg      = str;
	record.logStr   =  convertToString(record);

	logs.push_back(record);
	logToXml(record);
}

float simpleLogger::getWidth(){
   return 0;
}

float simpleLogger::getHeight(){
	return 0;
}

//----------------------------------------------
string simpleLogger::convertToString(logRecord & record){
	if(bDate) record.dateStr = ofToString(record.year) +"-"+ofToString(record.month) +"-"+ ofToString(record.day)+" ";
	if(bTime) record.timeStr = ofToString(record.hour) + ":"+ ofToString(record.minute) + ":"+ ofToString(record.seconds)+" ";

	if( bLevel ){
		if(record.level == OF_LOG_VERBOSE) record.levelStr = "VERBOSE: ";
		else if(record.level == OF_LOG_NOTICE)record.levelStr = "NOTICE: ";
		else if(record.level == OF_LOG_WARNING)record.levelStr = "WARNING: ";
		else if(record.level == OF_LOG_ERROR)record.levelStr = "ERROR: ";
		else if(record.level == OF_LOG_FATAL_ERROR)record.levelStr = "FATAL_ERROR: ";
	}

	return record.dateStr + record.timeStr + record.levelStr + record.msg;
}

//-----------------------------------------------
void simpleLogger::logToXml(logRecord & record){
	if(!fileLoaded)return;
	xml.addValue("log", record.logStr);
}

//-----------------------------------------------.
void simpleLogger::saveFile(){
	xml.saveFile(logFile);
}

//-----------------------------------------------.
void simpleLogger::draw(float x, float y){
	ofPushStyle();
		float yPos;
		for(int i = logs.size()-1; i >= 0; i--){
			yPos += 13.6;
			string str = logs[i].logStr;
			ofDrawBitmapString(str, x, y + yPos);
		}
	ofPopStyle();
}

//-----------------------------------------------.
void simpleLogger::draw(float x, float y, float width, float height){
	ofPushStyle();
		float yPos;
		for(int i = logs.size()-1; i >= 0; i--){
			yPos += 13.6;
			if(yPos >= height)break;

			string str = logs[i].logStr;
			if( str.length() * 8> width ){
				int newLen = (float)width / 8;
				//newLen = ofClamp(newLen, 1, 999999);
				str = str.substr(0, newLen);
			}

			ofDrawBitmapString(str, x, y + yPos);
		}
	ofPopStyle();
}