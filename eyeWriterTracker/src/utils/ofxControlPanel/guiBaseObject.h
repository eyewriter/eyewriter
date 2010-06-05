#pragma once

#include "simpleColor.h"
#include "guiColor.h"
#include "guiValue.h"
#include "guiTextBase.h"
#include "guiCallback.h"

typedef enum{
SG_STATE_NORMAL,
SG_STATE_SELECTED
}sgState;

typedef enum{
	SG_TYPE_FLOAT,
	SG_TYPE_INT,
	SG_TYPE_BOOL
}sgType;

static bool isInsideRect(float x, float y, ofRectangle rect){
    return ( x >= rect.x && x <= rect.x + rect.width && y >= rect.y && y <= rect.y + rect.height );
}

class guiBaseObject{

    public:
        guiBaseObject();

        //------------------------------------------
        virtual void setXmlName(string _xmlName);

        //--------------------------------------------
        virtual void setFont(ofTrueTypeFont * fontPtr);

        //should be called on mousedown
        //-------------------------------------------
        virtual bool checkHit(float x, float y, bool isRelative);

        //this is the equivilant of mouse moved if the gui element has been selected
        //this is empty as it really should be specified by
        //the extending class (gui element).
        //------------------------------------------------
        virtual void updateGui(float x, float y, bool firstHit, bool isRelative);

        //should  be called on mouse up
        //-------------------------------------------
        virtual void release();
		
        //these are here for the custom control types
        //we notify all elements about these actions
        virtual void saveSettings(string filename);
        virtual void reloadSettings();
        virtual void saveSettings();
        virtual void loadSettings(string filename);

        virtual void lock();
        virtual void unlock();
        bool isLocked();

        //------------------------------------------------
        virtual void setShowText(bool showText);
		
        //-----------------------------------------------
        virtual void setTypeInt();

        //-----------------------------------------------
        virtual void setTypeFloat();
		
        //-----------------------------------------------
        virtual void setTypeBool();
		
        //-----------------------------------------------
        virtual void setPosition(float x, float y);

        //------------------------------------------------
        virtual void setDimensions(float width, float height);

        //-----------------------------------------------
        virtual float getPosX();
		
        //------------------------------------------------
        virtual float getPosY();
		
        //------------------------------------------------
        virtual float getWidth();

        //-----------------------------------------------
        virtual float getHeight();

        //-----------------------------------------------
        virtual void update();

		//-----------------------------------------------
		virtual void notify();
		
        //-----------------------------------------------
        virtual void checkPrescison();
		
//        //-----------------------------------------------
//        virtual void setIsRelative( bool bIsRelative ){
//            isRelative = bIsRelative;
//        }

        //every time we update the value of our text
        //-----------------------------------------------
        virtual void updateText();

        //---------------------------------------------
        virtual void updateBoundingBox();

        //---------------------------------------------
        virtual void renderText();

        //this also needs to be specified by the extending class
        //--------------------------------------------
        virtual void render();

         //-------------------------------------------
         virtual void setSelected();

         //-------------------------------------------
         virtual void setNormal();
		 
         //-------------------------------------------
         virtual void setForegroundColor( int norR, int norG, int norB, int norA);

         //-------------------------------------------
         virtual void setForegroundSelectColor(int selR, int selG, int selB, int selA);

         //-------------------------------------------
         virtual void setBackgroundColor( int norR, int norG, int norB, int norA);

         //-------------------------------------------
         virtual void setBackgroundSelectColor(int selR, int selG, int selB, int selA);

         //-------------------------------------------
         virtual void setOutlineColor( int norR, int norG, int norB, int norA);

         //-------------------------------------------
         virtual void setOutlineSelectColor(int selR, int selG, int selB, int selA);

         //-------------------------------------------
         virtual void setTextColor( int norR, int norG, int norB, int norA);

         //-------------------------------------------
         virtual void setTextSelectColor(int selR, int selG, int selB, int selA);

         virtual void setValue(float _value, int whichParam);

        virtual void updateValue();

        //list of properties
        //------------------
        string name;
        string drawStr;
        string xmlName;

        ofRectangle boundingBox;
        ofRectangle hitArea;

        guiColor fgColor;
        guiColor bgColor;
        guiColor outlineColor;
        guiColor textColor;

        int numDecimalPlaces;

        //------------------
        guiTextBase displayText;

        //-------------------
        vector <guiBaseObject *>children;

		ofEvent <guiCallbackData> guiEvent;		

        //bool isRelative;
        bool locked;
		bool bRemoveFromLayout;

        float storedTextWidth;
        float titleSpacing;
        float fontSize;
        bool bShowText;
        bool readOnly;
        int  state;
        int  dataType;

//protected:

        guiValue value;

};

