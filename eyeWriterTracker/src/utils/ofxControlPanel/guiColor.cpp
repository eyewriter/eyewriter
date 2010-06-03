#include "guiColor.h"

//------------------------------------------------
guiColor::guiColor(){
	bUseSelected  = false;
	bUseDisabled    = false;
	colorMode       = 0;
}

//------------------------------------------------
void guiColor::setColor(int r, int g, int b, int a){
	color.setColor(r, g, b, a);
}

//------------------------------------------------
void guiColor::setColor(int hexValue){
	color.setColor(hexValue);
}

//------------------------------------------------
void guiColor::setSelectedColor(int r, int g, int b, int a){
	selected.setColor(r, g, b, a);
	bUseSelected = true;
}

//------------------------------------------------
void guiColor::setSelectedColor(int hexValue){
	selected.setColor(hexValue);
	bUseSelected = true;
}

//------------------------------------------------
void guiColor::setDisabledColor(int r, int g, int b, int a){
	disabled.setColor(r, g, b, a);
	bUseDisabled = true;
}

//------------------------------------------------
void guiColor::setDisabledColor(int hexValue){
	disabled.setColor(hexValue);
	bUseDisabled = true;
}

//------------------------------------------------
void guiColor::setGuiColorMode(int whichColor){
	colorMode = whichColor;
}

//----------------------------------------------------------
simpleColor& guiColor::getColor(){
	if( colorMode == 1 && bUseSelected) return selected.getColor();
	else if( colorMode == 2 && bUseDisabled ) return disabled.getColor();
	else return color.getColor();
}

//----------------------------------------------------------
float* guiColor::getColorF(){
	if( colorMode == 1 && bUseSelected) return selected.getColorF();
	else if( colorMode == 2 && bUseDisabled) return disabled.getColorF();
	else return color.getColorF();
}

//----------------------------------------------------------
float* guiColor::getSelectedColorF(){
	return selected.getColorF();
}

//----------------------------------------------------------
float* guiColor::getNormalColorF(){
   return color.getColorF();
}
