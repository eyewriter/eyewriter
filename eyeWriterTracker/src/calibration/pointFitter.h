#pragma once

#include "ofMain.h"
#include "ofxLeastSquares.h"
#include "calibrationFileSaver.h"

class pointFitter {
	
public:

	void setup(int _nDivisionsWidth, int _nDivisionsHeight);
	void update(int _currentPoint, float _xp, float _yp);
	ofPoint getCalibratedPoint (float x, float y);

	void draw();
	void drawLsError();
	void drawRawCalibrationInput(int offsetX, int offsetY, float scale);
	void drawNonCalibration(ofRectangle & calibrationRectangle);
	
	void clear();
	
	void startCalibration();
	vector<float> makeOutput(float x, float y);
	vector<float> makeInput(float x, float y);
	void removeOutliers();
	void removePointsFarFromAverage();
	void calculateErrors(ofRectangle & calibrationRectangle);

	void registerCalibrationInput(float x, float y);

	// calculate the weights for an equation that goes from tracked points to known points.
	void calculateWeights(vector <ofPoint> trackedPoints, vector <ofPoint> knownPoints);

	void saveCalibration();
	void loadCalibration();
	void calculate(ofRectangle & calibrationRectangle);
	
	bool numberThreshoodForEachPoint(int threshold);
	
	
	ofxLeastSquares	ls;
	int	inputCount;
	int	outputCount;
	float removeOutliersf, lastRemoveOutliers;

	float xp, yp;
	bool bDrawLsError;
	bool bDrawRawCalibrationInput;
	ofPoint	rawDataOffset;
	float rawDataScale;
	
	bool bBeenFit;
	int nDivisionsWidth, nDivisionsHeight;
	int	currentPoint;				//current calibrating point
	
	vector <int> nPtsInPoints;
	vector <ofPoint> pointsFromls;
	vector <ofPoint> referencePoints;
	vector <ofPoint> screenPoints;
	vector <ofPoint> eyePoints;
	
	calibrationFileSaver calibFileSaver;
	
};

