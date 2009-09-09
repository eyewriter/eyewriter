#include "calibrator.h"

#include <gsl_math.h>
#include <gsl_test.h>
#include <gsl_fit.h>
#include <gsl_multifit.h>
#include <gsl_multifit_nlin.h>
#include <gsl_blas.h>
#include <gsl_ieee_utils.h>


//--------------------------------------------------------------
void calibrator::setup(){
	
	
	bBeenFit = false;
	
	
	ofSetFullscreen(true);
	nDivisionsWidth		= 6;
	nDivisionsHeight	= 4;
	
	nPosition = 0;
	
	
	int nDivisionsWidth;
	int nDivisionsHeight;
	int nPosition;
	pos  = 0;
	inputEnergy = 0;
	
	bAutomatic = false;
	bAmInAutodrive = false;
	startTime = ofGetElapsedTimef();
	preTimePerDot = 2.0;
	recordTimePerDot = 1.5;
	totalTimePerDot = preTimePerDot + recordTimePerDot;
	bInAutoRecording = false;
	autoPct = 0;
}


//--------------------------------------------------------------
void calibrator::start(){
	bAutomatic = true;
	bAmInAutodrive = true;
	startTime = ofGetElapsedTimef();
}


void calibrator::saveCalibration(){
	ofxXmlSettings xml;
	for (int i = 0; i < 6; i++){
		xml.addTag("pt");
		xml.pushTag("pt", i);
		xml.addValue("x", cxfit[i]*10000);
		xml.addValue("y", cyfit[i]*10000);
		xml.popTag();
	}
	xml.saveFile("calibration.xml");
	
}

//--------------------------------------------------------------
void calibrator::loadCalibration(){
	
	
	
	ofxXmlSettings xml;
	xml.loadFile("calibration.xml");
	for (int i = 0; i < 6; i++){
		xml.pushTag("pt", i);
		cxfit[i] = xml.getValue("x", 0.0f ) / 10000.0f;
		cyfit[i] = xml.getValue("y", 0.0f )/ 10000.0f;
		xml.popTag();
	}
	bBeenFit = true;
	
	
	printf("-------------------------------------------- \n");
	
	
	
	
	for (int i = 0; i < 6; i++){
		printf("cx %i = %f \n", i, cxfit[i]);
	}
	
	for (int i = 0; i < 6; i++){
		printf("cy %i = %f \n", i, cyfit[i]);
	}
	
	
	
	printf("-------------------------------------------- \n");
	
	
	
}


//--------------------------------------------------------------
void calibrator::update(){
	
	// do the auto: 
	
	if (bAutomatic == true && bAmInAutodrive == true){
		int nPts = nDivisionsWidth * nDivisionsHeight;
		float totalTime = totalTimePerDot * nPts;
		if (ofGetElapsedTimef() - startTime > totalTime){
			bAmInAutodrive = false;
			bInAutoRecording = false;
		} else {
			
			float diffTime = ofGetElapsedTimef() - startTime ;
			int pt = (int)(diffTime / totalTimePerDot);
			float diffDotTime = diffTime - pt * totalTimePerDot;
			//cout << diffTime << " " << pt <<  " " << diffDotTime << endl;
			if (diffDotTime < preTimePerDot){
				
				autoPct = (diffDotTime / preTimePerDot);
				bInAutoRecording = false;
				
				
			} else{
				
				autoPct = (diffDotTime - preTimePerDot) / recordTimePerDot;
				
				bInAutoRecording = true;
			}
			pos = pt;
		}
		
		
		
	}
	
	inputEnergy *= 0.98f;
	
	int xx = (pos % nDivisionsWidth);
	int yy = (pos / nDivisionsWidth);
	
	xp = ((float)ofGetWidth() / (float)(nDivisionsWidth-1)) * xx;
	yp = ofGetHeight() - ((float)ofGetHeight() / (float)(nDivisionsHeight-1)) * yy;
	
}


//--------------------------------------------------------------
void calibrator::registerCalibrationInput(float x, float y){
		follow.push_back(ofPoint(xp,yp));
		myMouse.push_back(ofPoint(x, y));	
	
	
	inputEnergy = 1;
}

//--------------------------------------------------------------
void calibrator::draw(){
	
	// draw a light grid: 
	ofSetColor(255, 255, 255, 40);
	for (int i = 0; i < nDivisionsWidth; i++){
		float xp = ((float)ofGetWidth() / (float)(nDivisionsWidth-1)) * i;
		ofLine(xp, 0,xp, ofGetHeight());
	}
	
	for (int i = 0; i < nDivisionsHeight; i++){
		float yp = ofGetHeight() - ((float)ofGetHeight() / (float)(nDivisionsHeight-1)) * i;
		ofLine(0,yp, ofGetWidth(),yp);
	}
	
	
	
	
	
	
		
	ofEnableAlphaBlending();
	
	float rad = 30;
	glLineWidth(4);
	ofSetColor(255, 255, 255, 100);
	ofLine (xp, yp-rad, xp, yp+rad);
	ofLine (xp-rad, yp, xp+rad, yp);;
	
	
	ofSetColor(255, 255, 255, 255);

	if (bAutomatic == true && bAmInAutodrive == true && bInAutoRecording){
	} else {
		ofCircle(xp, yp, rad*0.33);
	}
	
	ofFill();
	
	// 
	// TODO: do some animation stuff here: 
	if (bAutomatic == true && bAmInAutodrive == true){
		
		if (bInAutoRecording){
			
			ofSetColor(255, 0, 0, 200);
			
			ofCircle(xp, yp, 30 + 4 * fabs(sin(autoPct * 8)));
			
		} else {
			
			ofSetColor(255, 255, 255, 150);
			ofCircle(xp, yp, 200 - 200* autoPct);
			
		}
		
	}
	
	
	glLineWidth(1);
	ofFill();
	/*if (inputEnergy > 0){	
		ofSetColor(255, 0, 127, 60*inputEnergy);
		ofCircle(xp, yp, rad * 3.5 );
		
		ofSetColor(255, 0, 127, 150*inputEnergy);
		ofCircle(xp, yp, rad );
	}*/



	
}


void calibrator::clear(){
	follow.clear();
	myMouse.clear();
	bBeenFit = false;
}

//--------------------------------------------------------------
void calibrator::calculate(){
	calculateWeights(myMouse, follow);
	inputEnergy = 0;

}


//--------------------------------------------------------------
void calibrator::advancePosition(){
	pos++;
	pos %= (nDivisionsWidth*nDivisionsHeight);
	inputEnergy = 0;
}



void calibrator::calculateWeights(vector <ofPoint> eyePoints, vector <ofPoint>  screenPoints){
	
	int length = eyePoints.size();
	
	int nTerms = 6;
	
	gsl_matrix * x = gsl_matrix_alloc(length,nTerms);
	gsl_vector * yx = gsl_vector_alloc(length);
	gsl_vector * yy = gsl_vector_alloc(length);
	gsl_vector * w = gsl_vector_alloc(nTerms);
	
	double * ptr;
	double * ptrScreenX;
	double * ptrScreenY;

	
	ptr = gsl_matrix_ptr(x,0,0);
	ptrScreenX = gsl_vector_ptr(yx,0);
	ptrScreenY = gsl_vector_ptr(yy,0);
	
	
	for (int i = 0; i < length; i++){

		float xPosEye = eyePoints[i].x;
		float yPosEye = eyePoints[i].y;
		
		
		// Ax + Bx^2 + Cy + Dy^2 + Exy + Fx^3 + Gy^3 + H
		
		*ptr++ = xPosEye;
		*ptr++ = xPosEye*xPosEye;
		*ptr++ = yPosEye;
		*ptr++ = yPosEye*yPosEye;
		*ptr++ = xPosEye*yPosEye;
		//*ptr++ = xPosEye*xPosEye*xPosEye;
		//*ptr++ = yPosEye*yPosEye*yPosEye;
		*ptr++ = 1;
		
		*ptrScreenX++ = screenPoints[i].x;
		*ptrScreenY++ = screenPoints[i].y;
		
	}
	
	
	gsl_vector *cx = gsl_vector_calloc(nTerms);
	gsl_vector *cy = gsl_vector_calloc(nTerms);
	
	
    gsl_matrix *cov = gsl_matrix_calloc(nTerms, nTerms); 
	double chisq;
	
	gsl_multifit_linear_workspace *work = gsl_multifit_linear_alloc(length, nTerms); 

	int res = gsl_multifit_linear (x,
								   yx,
								   cx,
								   cov,
								   &chisq,
								   work);
	
	int res2 = gsl_multifit_linear (x,
								   yy,
								   cy,
								   cov,
								   &chisq,
								   work);
	
	printf("-------------------------------------------- \n");
	
	
	double * xptr = gsl_vector_ptr(cx,0);
	double * yptr = gsl_vector_ptr(cy,0);

	for (int i = 0; i < nTerms; i++){
		printf("cx %i = %f \n", i, xptr[i]);
		cxfit[i] =  xptr[i];
	}
	
	for (int i = 0; i < nTerms; i++){
		printf("cy %i = %f \n", i, yptr[i]);
		cyfit[i] =  yptr[i];
	}
	
	
	bBeenFit = true;
	
	
	printf("-------------------------------------------- \n");
	
	
	//return ;
	
	
}


ofPoint calibrator::getCalibratedPoint (float x, float y){

	if (bBeenFit == true){
		float calibratedx = (x) * cxfit[0] + (x*x) * cxfit[1] + (y) * cxfit[2] + (y*y) * cxfit[3] 
		+ (x*y) * cxfit[4] + cxfit[5];			//+ (x*x*x) * cxfit[5] + (y*y*y) * cxfit[6] + cxfit[7];
		float calibratedy = (x) * cyfit[0] + (x*x) * cyfit[1] + (y) * cyfit[2] + (y*y) * cyfit[3] 
		+ (x*y) * cyfit[4] + cyfit[5];			//(x*x*x) * cyfit[5] + (y*y*y) * cyfit[6] + cyfit[7];
		
		// ---------------------------------------------------
		// let's fix "offscreen" or very bad values, since we are smoothing them in....
		if (calibratedx < -1000)	calibratedx = -1000;
		if (calibratedx > 2000)		calibratedx = 2000;
		if (calibratedy < -1000)	calibratedy = -1000;
		if (calibratedy > 2000)		calibratedy = 2000;
		
		if (isnan(calibratedx))		calibratedx = 0;
		if (isnan(calibratedy))		calibratedy = 0;
		// ---------------------------------------------------
		
		return ofPoint(calibratedx, calibratedy);
		
	}
	
	return ofPoint(0,0);
}


