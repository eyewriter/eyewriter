#include "ofxLeastSquares.h"

ofxLeastSquares::ofxLeastSquares() :
mapMat(NULL) {
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
}

void ofxLeastSquares::add(const float* input, const float* output) {
	vector<float> inputVec, outputVec;
	inputVec.resize(inputCount);
	outputVec.resize(outputCount);
	copy(input, input + inputCount, inputVec.begin());
	copy(output, output + outputCount, outputVec.begin());
	add(inputVec, outputVec);
}

void ofxLeastSquares::update() {
	int n = inputData.size();
	
	CvMat* inputMat = cvCreateMat(n, inputCount, CV_32FC1);
	CvMat* outputMat = cvCreateMat(n, outputCount, CV_32FC1);
	
	float* inputPtr = (float*) cvPtr1D(inputMat, 0);
	float* outputPtr = (float*) cvPtr1D(outputMat, 0);
	
	for(int i = 0; i < n; i++) {
		for(int j = 0; j < inputCount; j++)
			*(inputPtr++) = inputData[i][j];
		for(int j = 0; j < outputCount; j++)
			*(outputPtr++) = outputData[i][j];
	}
	
	cvSolve(inputMat, outputMat, mapMat, CV_SVD);
	
	cvReleaseMat(&inputMat);
	cvReleaseMat(&outputMat);
}

vector<float> ofxLeastSquares::map(vector<float> input) {
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
}
