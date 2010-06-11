#include "ofxLeastSquares.h"

ofxLeastSquares::ofxLeastSquares() :
		mapMat(NULL),
		errorUpdated(false) {
}

ofxLeastSquares::~ofxLeastSquares() {
	if(mapMat != NULL)
		cvReleaseMat(&mapMat);
}

void ofxLeastSquares::setup(int inputCount, int outputCount) {
	this->inputCount = inputCount;
	this->outputCount = outputCount;
	mapMat = cvCreateMat(inputCount, outputCount, CV_32FC1);
}

void ofxLeastSquares::add(vector<float> input, vector<float> output) {
	inputData.push_back(input);
	outputData.push_back(output);
	outliers.push_back(false);
	modelUpdated = false;
	errorUpdated = false;
}

void ofxLeastSquares::add(const float* input, const float* output) {
	vector<float> inputVec, outputVec;
	inputVec.resize(inputCount);
	outputVec.resize(outputCount);
	copy(input, input + inputCount, inputVec.begin());
	copy(output, output + outputCount, outputVec.begin());
	add(inputVec, outputVec);
}

void ofxLeastSquares::updateModel() {
	if(!modelUpdated) {
		int n = size();

		CvMat* inputMat = cvCreateMat(n, inputCount, CV_32FC1);
		CvMat* outputMat = cvCreateMat(n, outputCount, CV_32FC1);

		float* inputPtr = (float*) cvPtr1D(inputMat, 0);
		float* outputPtr = (float*) cvPtr1D(outputMat, 0);

		for(int i = 0; i < inputData.size(); i++) {
			if(!outliers[i]) {
				for(int j = 0; j < inputCount; j++)
					*(inputPtr++) = inputData[i][j];
				for(int j = 0; j < outputCount; j++)
					*(outputPtr++) = outputData[i][j];
			}
		}

		cvSolve(inputMat, outputMat, mapMat, CV_SVD);

		cvReleaseMat(&inputMat);
		cvReleaseMat(&outputMat);

		modelUpdated = true;
	}
}

vector<float> ofxLeastSquares::map(vector<float> input) {
	updateModel();

	CvMat* inMat = cvCreateMat(1, inputCount, CV_32FC1);
	float* inPtr = (float*) cvPtr1D(inMat, 0);
	for(int i = 0; i < inputCount; i++)
		*(inPtr++) = input[i];

	CvMat* outMat = cvCreateMat(1, outputCount, CV_32FC1);
	float* outPtr = (float*) cvPtr1D(outMat, 0);
	cvMatMul(inMat, mapMat, outMat);

	vector<float> result;
	result.resize(outputCount);
	for(int i = 0; i < outputCount; i++)
		result[i] = *(outPtr++);

	cvReleaseMat(&inMat);
	cvReleaseMat(&outMat);

	return result;
}

vector<float> ofxLeastSquares::map(const float* input) {
	vector<float> inputVec;
	inputVec.resize(inputCount);
	for(int i = 0; i < inputCount; i++)
		inputVec[i] = input[i];
	return map(inputVec);
}

void ofxLeastSquares::clear() {
	inputData.clear();
	outputData.clear();
	error.clear();
	outliers.clear();
	modelUpdated = false;
	errorUpdated = false;
}

void ofxLeastSquares::updateError() {
	if(!errorUpdated) {
		error.clear();
		for(int i = 0; i < inputData.size(); i++) {
			vector<float>& input = inputData[i];
			vector<float>& output = outputData[i];
			vector<float> reprojection = map(input);
			float curError = 0;
			for(int j = 0; j < outputCount; j++) {
				float delta = reprojection[j] - output[j];
				curError += delta * delta;
			}
			curError = sqrtf(curError);
			error.push_back(curError);
		}
		errorUpdated = true;
	}
}

void ofxLeastSquares::removeOutlier() {
	updateModel();
	updateError();
	float outlierError = 0;
	int outlierPosition = 0;
	for(int i = 0; i < error.size(); i++) {
		if(!outliers[i] && error[i] > outlierError) {
			outlierError = error[i];
			outlierPosition = i;
		}
	}
	outliers[outlierPosition] = true;
	modelUpdated = false;
	errorUpdated = false;
}

void ofxLeastSquares::resetOutliers() {
	for(int i = 0; i < outliers.size(); i++)
		outliers[i] = false;
}

/*
 This isn't quite right, because after every iteration you should mark
 everything as a non-outlier and get rid of the n worst outliers. But this
 will only make a difference in cases that have multiple strong interpretations.
*/
void ofxLeastSquares::removeOutliers(float standardDeviations) {
	updateError();
	float standardDeviation = getStandardDeviation();
	float target = standardDeviations * standardDeviation;
	while(getMaxError() > target && size() > inputCount)
		removeOutlier();
}

const vector<float>& ofxLeastSquares::getError() const {
	return error;
}

float ofxLeastSquares::getTotalError() const {
	float total = 0;
	for(int i = 0; i < error.size(); i++)
		total += error[i];
	return total;
}

float ofxLeastSquares::getMeanError() const {
	return getTotalError() / error.size();
}

float ofxLeastSquares::getStandardDeviation() const {
	float squares = 0;
	float mean = getMeanError();
	for(int i = 0; i < error.size(); i++) {
		float diff = error[i] - mean;
		squares += diff * diff;
	}
	return sqrtf(squares / error.size());
}

float ofxLeastSquares::getMaxError() const {
	float maxError = 0;
	for(int i = 0; i < outliers.size(); i++)
		if(!outliers[i])
			if(error[i] > maxError)
				maxError = error[i];
	return maxError;
}

const vector<bool>& ofxLeastSquares::getOutliers() const {
	return outliers;
}

vector< vector<float> > ofxLeastSquares::getMap() const {
	vector< vector<float> > map;
	float* mapPtr = (float*) cvPtr2D(mapMat, 0, 0);
	for(int i = 0; i < inputCount; i++) {
		vector<float> cur;
		for(int j = 0; j < outputCount; j++)
			cur.push_back(*(mapPtr++));
		map.push_back(cur);
	}
	return map;
}

void ofxLeastSquares::setMap(const vector< vector<float> > map) {
	float* mapPtr = (float*) cvPtr2D(mapMat, 0, 0);
	for(int i = 0; i < inputCount; i++) {
		const vector<float>& cur = map[i];
		for(int j = 0; j < outputCount; j++)
			*(mapPtr++) = cur[j];
	}
	modelUpdated = true;
}

int ofxLeastSquares::size() const {
	int n = 0;
	for(int i = 0; i < outliers.size(); i++)
		if(!outliers[i])
			n++;
	return n;
}

