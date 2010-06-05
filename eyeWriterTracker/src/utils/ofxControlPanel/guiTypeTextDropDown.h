#pragma once

#include "guiBaseObject.h"
#include "guiColor.h"
#include "simpleColor.h"
#include "guiValue.h"

class guiTypeTextDropDown : public guiBaseObject{

    public:

        //------------------------------------------------
        void setup(string dropDownName, int defaultBox, vector <string> boxNames);
        virtual void updateValue();
        void update();
        virtual void release();
        void updateGui(float x, float y, bool firstHit, bool isRelative);
        void render();

		void notify();

		vector <string> vecDropList;
		bool    bShowDropDown;
};

