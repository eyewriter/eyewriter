#pragma once

#include "guiBaseObject.h"
#include "guiColor.h"
#include "simpleColor.h"
#include "guiValue.h"

class guiType2DSlider : public guiBaseObject{

    public:

        guiType2DSlider();

        void setup(string sliderName, float defaultVal1, float min1, float max1, float defaultVal2, float min2, float max2);
        
		void updateGui(float x, float y, bool firstHit, bool isRelative);
        void setKnobSize(float _knobSize);
        
		virtual void setValue(float _value, int whichParam);
        virtual void updateValue();
		
		virtual void notify();	
		
        void render();

        float knobSize;

};
