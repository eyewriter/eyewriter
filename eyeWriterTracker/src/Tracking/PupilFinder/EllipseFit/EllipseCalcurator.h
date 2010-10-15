/*
 *  EllipseCalcurator.h
 *  EyeTrackDTTest
 *
 *  Created by itotaka on 5/18/10.
 *  Copyright 2010 YCAM. All rights reserved.
 *
 */

#ifndef _ELLIPSE_CALCURATOR_
#define _ELLIPSE_CALCURATOR_

#include "ofMain.h"
#include "FitEllipse.h"
#include "ofxOpenCv.h"
#include "ofxVectorMath.h"


class eyeEllipse {

public:

	float width;
	float height;

	float getWidth(){ return width; }
	float getHeight(){ return height; }
	float ellipseParam[5];	// ellipse parameters...

	void draw(float x, float y){
		draw(x,y, getWidth(), getHeight());
	}

	void draw(float x, float y, float w, float h){

		float axis_a = ellipseParam[0];
		float axis_b = ellipseParam[1];
		float cx 	 = ellipseParam[2];
		float cy 	 = ellipseParam[3];
		float theta	 = ellipseParam[4] * RAD_TO_DEG;
		float aspect = axis_b/axis_a;
		int resolution = 24;
		glPushMatrix();

		glTranslatef(x, y, 0);
		glScalef( (float)w/(float)getWidth(), (float)h/(float)getHeight(), 1);


		glTranslatef(cx,cy,0);
		glRotatef(theta, 0,0,1);

		glColor3f(1,0,1);
		glBegin(GL_LINE_LOOP);
		for (int i=0; i<resolution; i++){
			float t = TWO_PI * (float)i/(float)resolution;
			float ex = (axis_a * cos(t));
			float ey = (axis_b * sin(t));
			glVertex2f (ex,ey);
		}
		glEnd();


		glColor4f(1,0,1,0.1);
		glBegin(GL_POLYGON);
		for (int i=0; i<resolution; i++){
			float t = TWO_PI * (float)i/(float)resolution;
			float ex = (axis_a * cos(t));
			float ey = (axis_b * sin(t));
			glVertex2f (ex,ey);
		}
		glEnd();


		for (int i=0; i<resolution; i+=4){
			float t = TWO_PI * (float)i/(float)resolution;
			float ex = (axis_a * cos(t));
			float ey = (axis_b * sin(t));
			ofLine(0,0,ex,ey);
		}


		glPopMatrix();
	}

};



class ellipseCalcurator {

public:

	void	setup(int width, int height);					// picture width, it is not blob width.
	bool	update(ofxCvBlob	& blob);
	void	draw(float x, float y);
	void	draw(float x, float y, float w, float h);

	CvMemStorage *	storage;

	ofPoint			currentEyePoint;
	ofPoint			currentNormPoint;

	eyeEllipse			eyeTrackedEllipse;


};

#endif
