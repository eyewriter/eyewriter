#pragma once

#include "guiColor.h"
#include "simpleColor.h"
#include "guiValue.h"

class guiTextBase{

    public:

        //-------------------------------------------
        guiTextBase();
        virtual void setFont(ofTrueTypeFont * fontPtr);

		virtual float getTextWidth(string textIn);
		virtual float getTextWidth();
		
		virtual float getTextHeight();

		virtual float getTextSingleLineHeight();

		virtual void setText(string textStr); 

		virtual void renderText(float x, float y);
		virtual void renderString(string textToRender, float x, float y);

        virtual float stringHeight(string textToRender);
		virtual float stringWidth(string textToRender);

		virtual bool usingTTF(){
			return ourFont != NULL;
		}

        //--------------------------------------------
        ofTrueTypeFont * ourFont;

        string textString;

        int numNewLines;
        int fontSize;
        bool bRealFont;
        bool bShowText;
};
