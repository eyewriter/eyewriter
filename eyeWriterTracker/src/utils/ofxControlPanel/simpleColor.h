#pragma once

#include "ofMain.h"

class simpleColor{

    public:

    //----------------------------------------------------------
    simpleColor();

	//----------------------------------------------------------
	simpleColor(int _r, int _g, int _b, int _a);

	//----------------------------------------------------------
	simpleColor(int hexColor);

    //----------------------------------------------------------
    void setColor(int _r, int _g, int _b, int _a);

    //----------------------------------------------------------
    void setColor(int hexColor);

    //----------------------------------------------------------
    simpleColor& getColor();

    //----------------------------------------------------------
    float * getColorF();

    //------------------
    //------------------
    union  {
        struct {
            float r;
            float g;
            float b;
            float a;
        };
        float color[4];
    };

};


