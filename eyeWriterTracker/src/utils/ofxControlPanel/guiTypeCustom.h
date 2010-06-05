#pragma once

#include "guiBaseObject.h"
#include "guiColor.h"
#include "simpleColor.h"
#include "guiValue.h"
#include "guiCustomImpl.h"

class guiTypeCustom : public guiBaseObject{

     public:

        guiTypeCustom();
        void setup(string customName, guiCustomImpl * customIn, float panelWidth, float panelHeight);
        virtual void saveSettings();
        virtual void saveSettings(string filename);
        virtual void loadSettings(string filename);
        virtual void reloadSettings();
        virtual void update();
        void updateGui(float x, float y, bool firstHit, bool isRelative);
        virtual void release();
        void render();

        guiCustomImpl * custom;
};
