#pragma once

#include "ofxOpenCv.h"

/*
 ofxLeastSquares generates a mapping function that models an unknown     system
 based on example input and output measurements. There are 4 steps:
 
 1 setup() how many input and output variables there are
 2 add() your example input and output data
 3 update() the mapping function based on all the data
 4 map() new input to modeled output
 */

/*
 Todo: use * for most code, and pass vector::iterators to them for speed.
 */

class ofxLeastSquares {
public:
	ofxLeastSquares();
	~ofxLeastSquares();
	
	void setup(int inputCount, int outputCount);
	
	void add(vector<float> input, vector<float> output);
	void add(const float* input, const float* output);
	
	void update();
	
	vector<float> map(vector<float> input);
	vector<float> map(const float* input);
	
	void clear();
	
protected:
	int inputCount, outputCount;
	vector< vector<float> > inputData, outputData;
	CvMat* mapMat;
};
