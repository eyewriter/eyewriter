#pragma once

#include "guiBaseObject.h"
#include "guiColor.h"
#include "simpleColor.h"
#include "guiValue.h"

#define LOCK_WIDTH 10
#define LOCK_HEIGHT 10
#define LOCK_BORDER 3

class guiTypePanel : public guiBaseObject{

    public:

        //------------------------------------------------
        guiTypePanel();

        void setup(string panelName, float defaultX = 20, float defaultY = 20);
        void addColumn(float minWidth);

        bool selectColumn(int which);
        void setElementSpacing(float spacingX, float spacingY);
        virtual bool checkHit(float x, float y, bool isRelative);

        void updateGui(float x, float y, bool firstHit, bool isRelative);
        virtual void update();

        void addElement( guiBaseObject * element );
        void drawLocked();
        void drawUnlocked();
        void render();

        ofRectangle lockRect;

        float currentXPos;
        float currentYPos;
        float spacingAmntX;
        float spacingAmntY;

        vector <ofRectangle> columns;
        vector <int> whichColumn;
        int col;

};
