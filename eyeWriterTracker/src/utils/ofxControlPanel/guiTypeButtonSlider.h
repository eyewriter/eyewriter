#pragma once


#include "guiBaseObject.h"
#include "guiColor.h"
#include "simpleColor.h"
#include "guiValue.h"
#include "guiTypeToggle.h"
#include "guiTypeSlider.h"

#define INCREMENT 0.005f

//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
class guiTypePlusButton : public guiTypeToggle{
    public:
        void render();
};

//-----------------------------------------------------------------------
//-----------------------------------------------------------------------

class guiTypeMinusButton : public guiTypeToggle{
    public:
        void render();
};

//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
class guiTypeButtonSlider : public guiBaseObject{


     public:

        guiTypeButtonSlider();
		
        ~guiTypeButtonSlider();

        //------------------------------------------------
        void setup(string buttonSliderName, float panelWidth, float panelHeight, float defaultVal, float min, float max, bool isInt);
		
        //-----------------------------------------------
        virtual void saveSettings();
        virtual void saveSettings(string filename);
		virtual void loadSettings(string filename);
        virtual void reloadSettings();

        virtual void setValue(float _value, int whichParam);
        virtual void updateValue();

        virtual void update();		
        void updateGui(float x, float y, bool firstHit, bool isRelative);
        virtual void release();
        void render();

        guiTypeSlider*      slider;
        guiTypeMinusButton* leftButton;
        guiTypePlusButton*  rightButton;
        ofPoint             prevMouse;
        bool                leftButtonOn;

        float 	            timeNow, timeThen;

};
