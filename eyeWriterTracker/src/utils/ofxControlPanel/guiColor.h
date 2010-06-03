#pragma once

#include "simpleColor.h"

class guiColor{

    public:

        //------------------------------------------------
        guiColor();
		void setColor(int r, int g, int b, int a);
		void setColor(int hexValue);
		void setSelectedColor(int r, int g, int b, int a);
		void setSelectedColor(int hexValue);
		void setDisabledColor(int r, int g, int b, int a);
		void setDisabledColor(int hexValue);
		void setGuiColorMode(int whichColor);
		
		simpleColor& getColor();
		
		float* getColorF();
		float* getSelectedColorF();
        float* getNormalColorF();

        //---------------
        //---------------
        simpleColor color;
        simpleColor selected;
        simpleColor disabled;

        int colorMode;
        bool bUseDisabled;
        bool bUseSelected;

};

