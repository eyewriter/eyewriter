#pragma once

#include "guiBaseObject.h"
#include "guiColor.h"
#include "simpleColor.h"
#include "guiValue.h"
#include "simpleFileLister.h"

class guiTypeFileLister : public guiBaseObject{

     public:

        guiTypeFileLister();
        void setup(string listerName, simpleFileLister * listerPtr , float listerWidth, float listerHeight);

        void updateGui(float x, float y, bool firstHit, bool isRelative);
        void release();
        void drawRecords(float x, float y, float width, float height);
        void render();

		void notify();
			
		int lineSpacing;
		int dblClickTime;			

        int selection;
        int selectionTmp;
        long lastClickTime;
        bool usingSlider;
        float sliderWidth;
        float selectPct;
        float pct;
        int startPos, endPos;
        simpleFileLister * lister;
};
