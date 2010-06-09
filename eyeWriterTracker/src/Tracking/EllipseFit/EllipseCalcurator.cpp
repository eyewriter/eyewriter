/*
 *  EllipseCalcurator.cpp
 *  EyeTrackDTTest
 *
 *  Created by itotaka on 5/18/10.
 *  Copyright 2010 YCAM. All rights reserved.
 *
 */


#include "EllipseCalcurator.h"
#include "FitEllipse.h"

//==========================================================
// solve_ellipse
// conic_param[6] is the parameters of a conic {a, b, c, d, e, f}; conic equation: ax^2 + bxy + cy^2 + dx + ey + f = 0;
// ellipse_param[5] is the parameters of an ellipse {ellipse_a, ellipse_b, cx, cy, theta}; a & b is the major or minor axis;
// cx & cy is the ellipse center; theta is the ellipse orientation
bool solve_ellipse (double* conic_param, double* ellipse_param){

	double a = conic_param[0];
	double b = conic_param[1];
	double c = conic_param[2];
	double d = conic_param[3];
	double e = conic_param[4];
	double f = conic_param[5];

	//get ellipse orientation
	double theta = atan2(b, a-c)/2;

	//get scaled major/minor axes
	double ct = cos(theta);
	double st = sin(theta);
	double ap = a*ct*ct + b*ct*st + c*st*st;
	double cp = a*st*st - b*ct*st + c*ct*ct;

	//get translations
	double cx = (2*c*d - b*e) / (b*b - 4*a*c);
	double cy = (2*a*e - b*d) / (b*b - 4*a*c);

	//get scale factor
	double val = a*cx*cx + b*cx*cy + c*cy*cy;
	double scale_inv = val - f;

	if (scale_inv/ap <= 0 || scale_inv/cp <= 0) {
		printf("Error! ellipse parameters are imaginary a=sqrt(%lf), b=sqrt(%lf)\n", scale_inv/ap, scale_inv/cp);
		memset(ellipse_param, 0, sizeof(double)*5);
		return false;
	}

	ellipse_param[0] = sqrt(scale_inv / ap);
	ellipse_param[1] = sqrt(scale_inv / cp);
	ellipse_param[2] = cx;
	ellipse_param[3] = cy;
	ellipse_param[4] = theta;
	return true;
}

//--------------------------------------------------------------

void ellipseCalcurator::setup(int width, int height){

	storage	= cvCreateMemStorage(0);

	eyeTrackedEllipse.width = width;
	eyeTrackedEllipse.height = height;

}
//--------------------------------------------------------------
bool ellipseCalcurator::update(ofxCvBlob & blob){

	// do some convex hull stuff:
	CvSeq* ptseq = cvCreateSeq( CV_SEQ_KIND_GENERIC|CV_32SC2, sizeof(CvContour),sizeof(CvPoint), storage );
	CvSeq* hull;
	CvPoint pt0;

	for(int i = 0; i < blob.nPts; i++ ){
		// this is kind of unfortunate, we're losing the accuracy of the blob points
		// before we do the ellipse fitting on them.
		pt0.x = (int) blob.pts[i].x;
		pt0.y = (int) blob.pts[i].y;
		cvSeqPush( ptseq, &pt0 );

	}
	hull = cvConvexHull2( ptseq, 0, CV_CLOCKWISE, 0 );				// put all points into cvConvexHull2
	int hullcount = hull->total;

	// -------------------------------- TRY TO GET A GOOD ELLIPSE HELLS YEAH !!

	int MYN = hullcount;
	float x[MYN], y[MYN];
	double p[6];
	double ellipseParam[5];
	float theta;
	FitEllipse fitter;
	for (int i=0; i<MYN; i++){											//Get all point from calcurated convex Hull.
		CvPoint pt = **CV_GET_SEQ_ELEM( CvPoint*, hull, i);
		x[i] = pt.x;
		y[i] =  pt.y;
	}
	double xc, yc;
	double xa, ya;
	double la, lb;
	fitter.apply(x,y,MYN);	// pointer to float array x&y& num of floats in Array.
	p[0] = fitter.Axx;
	p[1] = fitter.Axy;
	p[2] = fitter.Ayy;
	p[3] = fitter.Ax;
	p[4] = fitter.Ay;
	p[5] = fitter.Ao;

	bool bOk = solve_ellipse(p,ellipseParam);

	ofxCvBlob temp;

	if (bOk == true){

		//float *params_ellipse = pupilGeometries[whichEye].params_ellipse;
		float axis_a = ellipseParam[0];
		float axis_b = ellipseParam[1];
		float cx 	 = ellipseParam[2];
		float cy 	 = ellipseParam[3];
		theta	 = ellipseParam[4];
		float aspect = axis_b/axis_a;

		for (int i = 0; i < 5; i++){
			eyeTrackedEllipse.ellipseParam[i] = ellipseParam[i];
		}

		//theta = ofRandom(0,TWO_PI);


		int resolution = 24;

		ofxPoint2f ptsForRotation[resolution];

		for (int i=0; i<resolution; i++){
			float t = TWO_PI * (float)i/(float)resolution;
			float ex = cx + (axis_a * cos(t ));
			float ey = cy + (axis_b * sin(t ));
			ptsForRotation[i].set(ex,ey);
		}

		for (int i=0; i<resolution; i++){
			ptsForRotation[i].rotate(theta * RAD_TO_DEG, ofxPoint2f(cx, cy));
		}

		currentEyePoint.set(cx, cy);

	}

	cvRelease((void **)&hull);

	return bOk;

}
//--------------------------------------------------------------
void ellipseCalcurator::draw(float x, float y){

	eyeTrackedEllipse.draw(x,y);

}

//--------------------------------------------------------------
void ellipseCalcurator::draw(float x, float y, float w, float h){

	eyeTrackedEllipse.draw(x, y, w, h);

}





