#include "guiValue.h"

guiValue::guiValue(){

}

//------------------------------------------------
void guiValue::addValue(float val, float _min, float _max){
	value.push_back(val);
	valueI.push_back((int)val);
	valueB.push_back((bool)val);
	min.push_back(_min);
	max.push_back(_max);
	pct.push_back( 0.0 );

	//update our pct
	updatePct(value.size()-1);
}

//------------------------------------------------
void guiValue::addValueI(int val, int _min, int _max){
	value.push_back((float)val);
	valueI.push_back(val);
	valueB.push_back((bool)val);
	min.push_back((float)_min);
	max.push_back((float)_max);
	pct.push_back( 0.0 );

	//update our pct
	updatePct(value.size()-1);
}

//------------------------------------------------
void guiValue::addValueB(bool val){
	value.push_back((float)val);
	valueI.push_back((int)val);
	valueB.push_back(val);
	min.push_back(0);
	max.push_back(1);
	pct.push_back( 0.0 );

	//update our pct
	updatePct(value.size()-1);
}

//------------------------------------------------
bool guiValue::setValue(float val, unsigned int which){
	if(which >= 0 && which < value.size() ){

		if( max[which] - min[which] == 0 ) return false;

		//check out bounds
		if(val < min[which])        val = min[which];
		else if(val > max[which])   val = max[which];

		value[which]  =  val;
		valueI[which] = (int)val;
		valueB[which] = (bool)val;

		//recalulate our pct
		updatePct(which);

		return true;
	}
	return false;
}

//------------------------------------------------
bool guiValue::setValueAsPct(float percent, unsigned int which){
	if(which >= 0 && which < value.size() ){

		if( max[which] - min[which] == 0 ) return false;

		if(percent < 0.0) percent = 0.0;
		else if(percent > 1.0) percent = 1.0;

		pct[which]    =  percent;

		value[which]  =  ( pct[which] * ( max[which] - min[which] ) ) + min[which];
		valueI[which] = (int)value[which];
		valueB[which] = (bool)value[which];

		return true;
	}
	return false;
}

//------------------------------------------------
float guiValue::getValueF(unsigned int which){
	if(which >= 0 && which < value.size() ){
		return value[which];
	}
	return 0.0;
}

//------------------------------------------------
float guiValue::getValueI(unsigned int which){
	if(which >= 0 && which < valueI.size() ){
		return valueI[which];
	}
	return false;
}

//------------------------------------------------
float guiValue::getValueB(unsigned int which){
	if(which >= 0 && which < valueB.size() ){
		return valueB[which];
	}
	return false;
}

//------------------------------------------------
float guiValue::getMin(unsigned int which){
	if(which >= 0 && which < min.size() ){
		return min[which];
	}
	return 0.0;
}

//------------------------------------------------
float guiValue::getMax(unsigned int which){
	if(which >= 0 && which < max.size() ){
		return max[which];
	}
	return 0.0;
}

//------------------------------------------------
float guiValue::getPct(unsigned int which){
	if(which >= 0 && which < pct.size() ){
		return pct[which];
	}
	return 0.0;
}

//------------------------------------------------
int guiValue::getNumValues(){
	return (int) value.size();
}

//-----------------------------------------------
void guiValue::updatePct(int which){
	pct[which] = (value[which] - min[which]) / (max[which] - min[which]);
}

