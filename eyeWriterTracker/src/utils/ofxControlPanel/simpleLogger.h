#pragma once

#include "ofMain.h"
#include "ofxXmlSettings.h"

typedef struct{
    string msg;
    int level;

    string logStr;
	string dateStr;
	string timeStr;
	string levelStr;

    long timestamp;
    int year;
    int month;
    int day;
    int hour;
    int minute;
    int seconds;
}logRecord;

class simpleLogger : public ofBaseDraws{

     public:

        simpleLogger();
        ~simpleLogger();

        void setup(string logFileName, bool overwrite);
        void setIncludeDate(bool bIncludeDate);
        void setIncludeTime(bool bIncludeTime);
        void setIncludeLevelbool(bool  bIncludeLevel);

        //--------------------------------------------------
        void log(int logLevel, const char* format, ...);

        float getWidth();
        float getHeight();

        //----------------------------------------------
        string convertToString(logRecord & record);

        void logToXml(logRecord & record);
        void saveFile();

        void draw(float x, float y);
        void draw(float x, float y, float width, float height);

        string logFile;
        ofxXmlSettings xml;
        bool fileLoaded;

        bool bLevel;
        bool bTime;
        bool bDate;

        vector <logRecord> logs;
};
