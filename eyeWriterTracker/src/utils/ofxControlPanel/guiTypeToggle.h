#pragma once


#include "guiBaseObject.h"
#include "guiColor.h"
#include "simpleColor.h"
#include "guiValue.h"

class guiTypeToggle : public guiBaseObject{

    public:

        //------------------------------------------------
        void setup(string toggleName, bool defaultVal);
        virtual void updateValue();
		void updateGui(float x, float y, bool firstHit, bool isRelative);
        void render();


};
