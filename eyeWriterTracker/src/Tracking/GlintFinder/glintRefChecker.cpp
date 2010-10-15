#include "glintRefChecker.h"

void glintRefChecker::setup() {

	bNeedCalculate = false;

}

void glintRefChecker::addGlints(ofxCvBlob & refBlobLeft, ofxCvBlob & refBlobRight, bool bIsTopGlints) {

	glintUnit tempLeft;
	glintUnit tempRight;
	
	tempLeft.area = refBlobLeft.area;
	tempRight.area = refBlobRight.area;
	tempLeft.boundingRect = refBlobLeft.boundingRect;
	tempRight.boundingRect = refBlobRight.boundingRect;
	
	if (bIsTopGlints){
		glintHis[GLINT_ELEMENT_TOP].glints[GLINT_ELEMENT_LEFT].push_back(tempLeft);
		glintHis[GLINT_ELEMENT_TOP].glints[GLINT_ELEMENT_RIGHT].push_back(tempRight);
		if (glintHis[GLINT_ELEMENT_TOP].glints[GLINT_ELEMENT_LEFT].size() > 500){
			glintHis[GLINT_ELEMENT_TOP].glints[GLINT_ELEMENT_LEFT].erase(glintHis[GLINT_ELEMENT_TOP].glints[GLINT_ELEMENT_LEFT].begin());
			glintHis[GLINT_ELEMENT_TOP].glints[GLINT_ELEMENT_RIGHT].erase(glintHis[GLINT_ELEMENT_TOP].glints[GLINT_ELEMENT_RIGHT].begin());
		}
	} else {
		glintHis[GLINT_ELEMENT_BOTTOM].glints[GLINT_ELEMENT_LEFT].push_back(tempLeft);
		glintHis[GLINT_ELEMENT_BOTTOM].glints[GLINT_ELEMENT_RIGHT].push_back(tempRight);
		if (glintHis[GLINT_ELEMENT_BOTTOM].glints[GLINT_ELEMENT_LEFT].size() > 500){
			glintHis[GLINT_ELEMENT_BOTTOM].glints[GLINT_ELEMENT_LEFT].erase(glintHis[GLINT_ELEMENT_BOTTOM].glints[GLINT_ELEMENT_LEFT].begin());
			glintHis[GLINT_ELEMENT_BOTTOM].glints[GLINT_ELEMENT_RIGHT].erase(glintHis[GLINT_ELEMENT_BOTTOM].glints[GLINT_ELEMENT_RIGHT].begin());
		}
	}
	
	bNeedCalculate = true;
}

void glintRefChecker::calculateAverages() {
		
	for (int k = 0; k < 2; k++) {
		for (int i = 0; i < 2; i++) {
		
			float tempAvgArea = 0;
			float tempAvgWidth = 0;
			float tempAvgHeight = 0;
			
			for (int j = 0; j < glintHis[k].glints[i].size(); j++) {
				tempAvgArea += glintHis[k].glints[i][j].area;
				tempAvgWidth += glintHis[k].glints[i][j].boundingRect.width;
				tempAvgHeight += glintHis[k].glints[i][j].boundingRect.height;
			}
			glintHis[k].avgArea[i] = tempAvgArea / glintHis[k].glints[i].size();
			glintHis[k].avgWidth[i] = tempAvgWidth / glintHis[k].glints[i].size();
			glintHis[k].avgHeight[i] = tempAvgHeight / glintHis[k].glints[i].size();
		}
		
		float tempAvgDistance = 0;
		
		for (int j = 0; j < glintHis[k].glints[GLINT_ELEMENT_LEFT].size(); j++) {
			tempAvgDistance += glintHis[k].glints[GLINT_ELEMENT_RIGHT][j].boundingRect.x
							- (glintHis[k].glints[GLINT_ELEMENT_LEFT][j].boundingRect.x
							+ glintHis[k].glints[GLINT_ELEMENT_LEFT][j].boundingRect.width);
		}
		glintHis[k].avgDistance = tempAvgDistance / glintHis[k].glints[GLINT_ELEMENT_LEFT].size();
	}
	
	bNeedCalculate = false;
}

bool glintRefChecker::checkSize(ofxCvBlob & blob, int glintID, float minPct, float maxPct) {

	int vertPos;
	int horiPos;
	
	if (glintID < 2) {
		vertPos = GLINT_ELEMENT_BOTTOM;
		horiPos = glintID;
	} else {
		vertPos = GLINT_ELEMENT_TOP;
		horiPos = glintID - 2;
	}
	
	if (ofInRange(blob.boundingRect.width, glintHis[vertPos].avgWidth[horiPos] * minPct, glintHis[vertPos].avgWidth[horiPos] * maxPct)
		&& ofInRange(blob.boundingRect.height, glintHis[vertPos].avgHeight[horiPos] * minPct, glintHis[vertPos].avgWidth[horiPos] * maxPct)){
		return true;
	} else {
		return false;
	}
}

float glintRefChecker::getAverageDistance(bool bIsTopGlints) {

	if (bIsTopGlints)	return glintHis[GLINT_ELEMENT_TOP].avgDistance;
	else return glintHis[GLINT_ELEMENT_BOTTOM].avgDistance;
}

int	glintRefChecker::getnFrames(bool bIsTopGlints){
	
	if (bIsTopGlints) return glintHis[GLINT_ELEMENT_TOP].glints[GLINT_ELEMENT_LEFT].size();
	else return glintHis[GLINT_ELEMENT_BOTTOM].glints[GLINT_ELEMENT_LEFT].size();
}

void glintRefChecker::clear() {
	for (int i = 0; i < 2; i++) {
		for (int j = 0; j < 2; j++) {
			glintHis[i].glints[j].clear();
			glintHis[i].avgArea[j] = 0;
			glintHis[i].avgWidth[j] = 0;
			glintHis[i].avgHeight[j] = 0;
		}
		glintHis[i].avgDistance = 0;
	}
}


