#include "simpleColor.h"

//----------------------------------------------------------
simpleColor::simpleColor(){
	r = 1.0;
	g = 1.0;
	b = 1.0;
	a = 1.0;
}

//----------------------------------------------------------
simpleColor::simpleColor(int _r, int _g, int _b, int _a){
	setColor(_r, _g, _b, _a);
}

//----------------------------------------------------------
simpleColor::simpleColor(int hexColor){
	setColor(hexColor);
}

//----------------------------------------------------------
void simpleColor::setColor(int _r, int _g, int _b, int _a){
	r = (float)_r / 255.0f; r = MAX(0,MIN(r,1.0f));
	g = (float)_g / 255.0f; g = MAX(0,MIN(g,1.0f));
	b = (float)_b / 255.0f; b = MAX(0,MIN(b,1.0f));
	a = (float)_a / 255.0f; a = MAX(0,MIN(a,1.0f));
}

//----------------------------------------------------------
void simpleColor::setColor(int hexColor){
	int r = (hexColor >> 24)& 0xff;
	int g = (hexColor >> 16) & 0xff;
	int b = (hexColor >> 8) & 0xff;
	int a = (hexColor >> 0) & 0xff;

	setColor(r, g, b, a);
}

//----------------------------------------------------------
simpleColor& simpleColor::getColor(){
	return *this;
}

//----------------------------------------------------------
float * simpleColor::getColorF(){
	return color;
}



