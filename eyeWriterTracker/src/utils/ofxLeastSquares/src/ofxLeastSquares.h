#pragma once

#include "ofxOpenCv.h"

/*
	ofxLeastSquares generates a mapping function that models an unknown	system
	based on example input and output measurements. There are 3 steps:

	1 setup() how many input and output variables there are
	2 add() your example input and output data
	3 map() new input to modeled output

	If you want to prune your data, you can use removeOutliers. For example,
	removeOutliers(2) will iteratively remove everything but the first two
	standard deviations worth of outliers (updating the model after removing each
	outlier).
*/

class ofxLeastSquares {
public:
	ofxLeastSquares();
	~ofxLeastSquares();

	void setup(int inputCount, int outputCount);

	void add(vector<float> input, vector<float> output);
	void add(const float* input, const float* output);

	vector<float> map(vector<float> input);
	vector<float> map(const float* input);

	void clear();

	const vector<float>& getError() const;
	const vector<bool>& getOutliers() const;

	void resetOutliers();
	void removeOutliers(float standardDeviations);

	vector< vector<float> > getMap() const;
	void setMap(const vector< vector<float> > map);

protected:
	int inputCount, outputCount;
	CvMat* mapMat;

	vector< vector<float> > inputData, outputData;
	vector<bool> outliers;
	vector<float> error;

	bool modelUpdated, errorUpdated;
	void updateModel();
	void updateError();

	int size() const;
	float getTotalError() const;
	float getMeanError() const;
	float getStandardDeviation() const;
	float getMaxError() const;
	void removeOutlier();
};
