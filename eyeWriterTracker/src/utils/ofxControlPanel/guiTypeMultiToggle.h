#pragma once

#include "guiBaseObject.h"
#include "guiColor.h"
#include "simpleColor.h"
#include "guiValue.h"

class guiTypeMultiToggle : public guiBaseObject{

    public:
        static const int boxSpacing = 2;
        static const int boxSize = 14;

        //------------------------------------------------
        void setup(string multiToggleName, int defaultBox, vector <string> boxNames);

        //-----------------------------------------------
        virtual void updateValue();
        void updateGui(float x, float y, bool firstHit, bool isRelative);
        void render();
		
		vector <string> bNames;
};
