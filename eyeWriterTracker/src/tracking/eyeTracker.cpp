#include "eyeTracker.h"


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
eyeTracker::eyeTracker(){
	
	bFoundOne	= false;
	//scalef		= 1;
	//xoffset		= 0;
	//yoffset		= 0;

}


		

//--------------------------------------------------------------
void eyeTracker::setup(int width, int height){

	w = width;
	h = height;

	colorImg.allocate(w, h);
	
	grayImgPreWarp.allocate(w, h);
	grayImgPreModification.allocate(w, h);
	grayImg.allocate(w, h);
	threshImg.allocate(w, h);
	edgeMask.allocate(w, h);
	edgeMaskInverted.allocate(w,h);
	
	threshold			= 60;
	currentEyePoint		= 0;
	
	// filtering: 
	bUseGamma			= false;
	gamma				= 1.0f;
	
	bUseContrast		= false;
	contrast			= 0.2;
	brightness			= 0.0;
	
	
	bUseDilate			= false;
	nDilations			= 1;


	// for opencv hull
	storage	= cvCreateMemStorage(0);
	
	flipX = false;
	flipY = false;	
	
	edgeMaskStartPos.set(w/2, h/2);
	edgeMaskInnerRadius = 250;
	edgeMaskOuterRadius = 350;
	edgeMaskPixels = new unsigned char [ (int)(w * h) ];
	
	calculateEdgePixels();

}


//--------------------------------------------------------------
void eyeTracker::flip(bool bHorizontal, bool bVertical){
	flipX = bHorizontal; 
	flipY = bVertical;
}


//--------------------------------------------------------------
void eyeTracker::calculateEdgePixels(){
	for (int i = 0; i < w; i++){
		for (int j = 0; j < h; j++){
			float dx = i - edgeMaskStartPos.x;
			float dy = j - edgeMaskStartPos.y;
			float dist = sqrt(dx*dx + dy*dy);
			float pct = (dist - edgeMaskInnerRadius) / (edgeMaskOuterRadius - edgeMaskInnerRadius);
			if (pct < 0) pct = 0;
			if (pct > 1) pct = 1;
			edgeMaskPixels[(int)(j * w + i)] = pct * 255;
		}
	}	
	edgeMask.setFromPixels(edgeMaskPixels, w,h);
	
	edgeMaskInverted = edgeMask;
	edgeMaskInverted.invert();
}


//--------------------------------------------------------------
ofPoint eyeTracker::getEyePoint(){
	return currentEyePoint;
}

//--------------------------------------------------------------
void eyeTracker::update(ofxCvGrayscaleImage & grayImgFromCam, float threshold, float minSize, float maxSize,  float minSquareness){
	
	//threshold?
	//threshold = thresh;

	grayImgPreWarp.setFromPixels(grayImgFromCam.getPixels(), grayImgFromCam.width, grayImgFromCam.height);		// TODO: there's maybe an unnecessary grayscale image (and copy) here...
	
	if( flipX || flipY ){
		grayImgPreWarp.mirror(flipY, flipX);
	}

	/*  // before we were scaling and translating, but this is removed for now
	 
	 if (fabs(xoffset-1) > 0.1f || fabs(yoffset-1) > 0.1f){
		grayImgPreWarp.translate(xoffset, yoffset);
	}
	
	if (fabs(scalef-1) > 0.1f){
		grayImgPreWarp.scale(scalef, scalef);
	}*/
	
	grayImg = grayImgPreWarp;
	
	
	
	grayImgPreModification = grayImg;
	grayImg.blur(5);
		
	if (bUseContrast == true){
		grayImg.applyBrightnessContrast(brightness,contrast);
	}

	if (bUseGamma == true){
		grayImg.applyMinMaxGamma(gamma);
	}
	
	grayImg += edgeMask;
	
	threshImg = grayImg;	

	
	threshImg.contrastStretch();
	threshImg.threshold(threshold, true);

	
	// the dilation of a 640 x 480 image is very slow, so let's just do a ROI near the thing we like: 
	
	threshImg.setROI(currentEyePoint.x-50, currentEyePoint.y-50, 100,100);	// 200 pix ok?
	if (bUseDilate == true){	
		for (int i = 0; i < nDilations; i++){
			threshImg.dilate();
		}
	}
	threshImg.resetROI();
	
	
	
	bFoundOne = false;
		
	int		whoFound = -1;

	int num = contourFinder.findContours(threshImg, minSize, maxSize, 100, false, true);
	if( num ){
		
		for(int k = 0; k < num; k++){
			
			
			
			float ratio =	contourFinder.blobs[k].boundingRect.width < contourFinder.blobs[k].boundingRect.height ?
							contourFinder.blobs[k].boundingRect.width / contourFinder.blobs[k].boundingRect.height :
							contourFinder.blobs[k].boundingRect.height / contourFinder.blobs[k].boundingRect.width;
			
			float arcl = contourFinder.blobs[k].length;
			float area = contourFinder.blobs[k].area;
			float compactness = (float)((arcl*arcl/area)/FOUR_PI);
			
			if (bUseCompactnessTest	== true && compactness > maxCompactness){
				continue;
			}
				
			
			//printf("compactness %f \n", compactness);
			
			//lets ignore rectangular blobs
			if( ratio > minSquareness){
				currentEyePoint = contourFinder.blobs[k].centroid;
				currentNormPoint.x = currentEyePoint.x;
				currentNormPoint.y = currentEyePoint.y;
				
				currentNormPoint.x /= w;
				currentNormPoint.y /= h;

				
				bFoundOne = true;
				whoFound = k;
				
				break;
			}
		}		
	}
	
	if (bFoundOne && whoFound != -1){
	
		
		
		// do some convex hull stuff: 
		CvSeq* ptseq = cvCreateSeq( CV_SEQ_KIND_GENERIC|CV_32SC2, sizeof(CvContour),sizeof(CvPoint), storage );
        CvSeq* hull;
		CvPoint pt0;
		
        for(int i = 0; i < contourFinder.blobs[whoFound].nPts; i++ ){

            pt0.x = contourFinder.blobs[whoFound].pts[i].x;
            pt0.y = contourFinder.blobs[whoFound].pts[i].y;
            cvSeqPush( ptseq, &pt0 );
			
        }
        hull = cvConvexHull2( ptseq, 0, CV_CLOCKWISE, 0 );
        int hullcount = hull->total;
		
			
		
		// -------------------------------- TRY TO GET A GOOD ELLIPSE HELLS YEAH !!
	
		int MYN = hullcount;
		float x[MYN], y[MYN];
		double p[6];
		double ellipseParam[5];
		float theta;
		FitEllipse fitter;
		for (int i=0; i<MYN; i++){
			CvPoint pt = **CV_GET_SEQ_ELEM( CvPoint*, hull, i);
			x[i] = pt.x;
			y[i] =  pt.y;
		}
		double xc, yc;
		double xa, ya;
		double la, lb;
		fitter.apply(x,y,MYN);	
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
			currentNormPoint.x = currentEyePoint.x;
			currentNormPoint.y = currentEyePoint.y;
			currentNormPoint.x /= w;
			currentNormPoint.y /= h;
			
			
		} else {
			
					
			bFoundOne = false;
			
		}
		
		cvRelease((void **)&hull);
		
	}

}


//--------------------------------------------------------------
void eyeTracker::draw(float x, float y, float totalWidth, float totalHeight){


	ofPushMatrix();
		ofTranslate(x, y, 0);
		ofScale(totalWidth/(w*2), totalHeight/h, 1);
		
		ofSetColor(255, 255, 255);
		grayImg.draw(0,0);
		threshImg.draw(threshImg.width, 0);
		contourFinder.draw(threshImg.width, 0);
		
		if( currentEyePoint != 0 ){
			ofPushStyle();
				ofFill();
				ofSetColor(0, 0, 255);
				ofCircle( x + threshImg.width + currentNormPoint.x * threshImg.width, y + currentNormPoint.y * threshImg.height, 4);
			ofPopStyle();
		}
		
	ofPopMatrix();
	
	ofSetColor(0xffffff);
	ofDrawBitmapString("thresh is "+ofToString(threshold, 0), x, y + 10);
	
	

}


