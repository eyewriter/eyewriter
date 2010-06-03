#pragma once

#include "guiBaseObject.h"
#include "guiColor.h"
#include "simpleColor.h"
#include "guiValue.h"

//alows you to stack ofBaseDraw objects as layers on top of each other and then pass it to guiTypeDrawable::setup
//handy if you want to render drawing from one class ontop of a video of another class. 

class drawableStacker : public ofBaseDraws{
    public:

    void addDrawer(ofBaseDraws * drawer);

    void setWidth(float w);

    void setHeight(float h);

    float getWidth();

    float getHeight();

    void draw(float x, float y, float w, float h);
    void draw(float x, float y);

    vector <ofBaseDraws *> drawers;
    float width;
    float height;
};

//pass in any ofBaseDraws class ( videoPlayer, videoGrabber, image, or your own class ) 
class guiTypeDrawable : public guiBaseObject{

     public:

        guiTypeDrawable();

        void setup(string videoName, ofBaseDraws * vidIn, float videoWidth, float videoHeight);
        void updateGui(float x, float y, bool firstHit, bool isRelative);

        void render();

        ofBaseDraws * vid;
};

