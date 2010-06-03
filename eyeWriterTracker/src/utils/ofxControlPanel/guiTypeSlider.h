#pragma once


#include "guiBaseObject.h"
#include "guiColor.h"
#include "simpleColor.h"
#include "guiValue.h"

class guiTypeSlider : public guiBaseObject{

    public:

        //------------------------------------------------
        void setup(string sliderName, float defaultVal, float min, float max);

        virtual void updateValue();
		void updateGui(float x, float y, bool firstHit, bool isRelative = false);
		void render();

};
