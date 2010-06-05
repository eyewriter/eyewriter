#pragma once

#include "guiBaseObject.h"
#include "guiColor.h"
#include "simpleColor.h"
#include "guiValue.h"
#include "simpleLogger.h"

class guiTypeLogger : public guiBaseObject{

     public:

        guiTypeLogger();
        void setup(string loggerName, simpleLogger * logger, float loggerWidth, float loggerHeight);
        void updateGui(float x, float y, bool firstHit, bool isRelative);
        void drawRecords(float x, float y, float width, float height);
        void render();
		
		int toggle;
		float pct;
        simpleLogger * log;
};
