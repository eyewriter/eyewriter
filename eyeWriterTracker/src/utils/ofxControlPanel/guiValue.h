#pragma once

#include "ofMain.h"

class guiValue{

    public:
        guiValue();

       //------------------------------------------------
        void addValue(float val, float _min, float _max);

        //------------------------------------------------
        void addValueI(int val, int _min, int _max);

        //------------------------------------------------
        void addValueB(bool val);

       //------------------------------------------------
        bool setValue(float val, unsigned int which = 0);

        //------------------------------------------------
        bool setValueAsPct(float percent, unsigned int which = 0);

       //------------------------------------------------
        float getValueF(unsigned int which = 0);

       //------------------------------------------------
        float getValueI(unsigned int which = 0);

       //------------------------------------------------
        float getValueB(unsigned int which = 0);

        //------------------------------------------------
        float getMin(unsigned int which = 0);

        //------------------------------------------------
        float getMax(unsigned int which = 0);

        //------------------------------------------------
        float getPct(unsigned int which = 0);

        int getNumValues();

    protected:

        //-----------------------------------------------
        void updatePct(int which);

        //--------------
        //--------------
        vector <float> value;
        vector <float> valueI;
        vector <float> valueB;

        vector <float> min;
        vector <float> max;
        vector <float> pct;

};

