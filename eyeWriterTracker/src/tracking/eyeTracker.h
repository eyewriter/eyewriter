#pragma once


#include "ofMain.h"
#include "ofxOpenCv.h"
#include "ofxCvGrayscaleAdvanced.h"
#include "ofxVectorMath.h"

class eyeEllipse {
	
	public:
	
	float getWidth(){	return 640; }
	float getHeight(){	return 480; }
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


class eyeTracker{

	public:

		eyeTracker();
		void setup(int width, int height);
		void flip(bool bHorizontal, bool bVertical);
		void update(ofxCvGrayscaleImage & grayImgFromCam, float threshold, float minSize, float maxSize,  float minSquareness = 0.5);
		void draw(float x, float y, float w, float h);
	
		ofPoint getEyePoint();
		
		void setDownTime(int downWaitTimeMillis);
		void setUpTime(int upWaitTimeMillis);
	
		ofxCvGrayscaleImage			grayImgPreModification;
		
		/*float scalef;
		float xoffset;
		float yoffset;*/
	
		ofxCvGrayscaleAdvanced		grayImgPreWarp;
	
		ofxCvGrayscaleAdvanced		grayImg;
		ofxCvColorImage				colorImg;
		ofxCvGrayscaleAdvanced		threshImg;
		ofxCvContourFinder			contourFinder;
	
		void						calculateEdgePixels();
		ofPoint						edgeMaskStartPos;
		float						edgeMaskInnerRadius;			// < innerRadius = white
		float						edgeMaskOuterRadius;			// > inner radius and < outer radius = fade to black
		unsigned char				* edgeMaskPixels;				// > outer radius = black.....
		ofxCvGrayscaleImage			edgeMask;
		ofxCvGrayscaleImage			edgeMaskInverted;				// for display, easier to see what's going on. 
	
		bool						bFoundOne;

	
		CvMemStorage *				storage;
	
		eyeEllipse					eyeTrackedEllipse;
	
		bool						bUseCompactnessTest;
		float						maxCompactness;
	

		
		// filtering / options:
		bool		bUseGamma;
		float		gamma;
		
		bool		bUseContrast;
		float		contrast;
		float		brightness;
		
		bool		bUseDilate;
		int			nDilations;

		bool		flipX, flipY;
		
		float		w, h;
		float		threshold;
		
		ofPoint currentEyePoint;
		ofPoint currentNormPoint;
		
};

