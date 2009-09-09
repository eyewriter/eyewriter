

#include "ofxCvGrayscaleAdvanced.h"


ofxCvGrayscaleAdvanced::ofxCvGrayscaleAdvanced(){
	//mapping from input to output values via lookup table, recalculated when brightness or contrast changed
	//lutmat = cvCreateMatHeader( 1, 256, CV_8UC1 );
	gammaLutMatrix		= cvCreateMatHeader( 1, 256, CV_8UC1 );
	briConLutMatrix		= cvCreateMatHeader( 1, 256, CV_8UC1 );
}

//--------------------------------------------------------------------------------
void  ofxCvGrayscaleAdvanced::drawBlobIntoMe( ofxCvBlob &blob, int color ) {
	if( blob.pts.size() > 0 ) {
		CvPoint* pts = new CvPoint[blob.nPts];
		for( int i=0; i < blob.nPts ; i++ ) {
			pts[i].x = (int)blob.pts[i].x;
			pts[i].y = (int)blob.pts[i].y;
		}
		int nPts = blob.nPts;
		cvFillPoly( cvImage, &pts, &nPts, 1,
				   CV_RGB(color,color,color) );
		delete pts;
	}
}


void ofxCvGrayscaleAdvanced::swapTemp() {
	
	if (getROI().x != 0 ||
		getROI().y != 0 ||
		getROI().width != cvImage->width ||
		getROI().height != cvImage->height ){
		cvCopy( cvImageTemp, cvImage );
	} else {	
		IplImage*  temp;
		temp = cvImage;
		cvImage	= cvImageTemp;
		cvImageTemp	= temp;
	}
}


void ofxCvGrayscaleAdvanced::applyMinMaxGamma(float gamma){
	
	// for this, we will search for the minimum and maximum values, and apply a gamma, ie: 
	
	// (a) search for min and max
	// (b) pct = val - min / max - min
	// (c) newVal = pow(pct, gamma) * 255
	//
	// k ??
	
	double minval;
	double maxval;
	
	if (cvImage != NULL){
		cvMinMaxLoc(cvImage,&minval,&maxval);
	}
	
	//printf("%i %i ", (int)minval,  (int)maxval);
	
	float spread = (float)(maxval - minval);
	
	
	
	for (int i=0; i<256; i++){
		float pct = (float) (i - minval) / spread;
		if (pct < 0) pct = 0;
		if (pct > 1) pct = 1;
		gammaLut[i] = (int)(255.0 * powf(pct, gamma));
	}
	cvSetData( gammaLutMatrix, gammaLut, 0 );
	
	
	//CvMat lutTemp = cvMat(1,256,CV_8UC1,lut);
	
	
	cvLUT( cvImage, cvImageTemp, gammaLutMatrix); 
	swapTemp();
    flagImageChanged();

}


void ofxCvGrayscaleAdvanced::applyBrightnessContrast(float brightness, float contrast){

	int i;
	
	/*
	 * The algorithm is by Werner D. Streidt
	 * (http://visca.com/ffactory/archives/5-99/msg00021.html)
	 */
	if( contrast > 0 )
	{
		double delta = 127.*contrast;
		double a = 255./(255. - delta*2);
		double b = a*(brightness*100 - delta);
		for( i = 0; i < 256; i++ )
		{
			int v = cvRound(a*i + b);
			if( v < 0 )
				v = 0;
			if( v > 255 )
				v = 255;
			briConLut[i] = (uchar)v;
		}
	}
	else
	{
		double delta = -128.*contrast;
		double a = (256.-delta*2)/255.;
		double b = a*brightness*100. + delta;
		for( i = 0; i < 256; i++ )
		{
			int v = cvRound(a*i + b);
			if( v < 0 )
				v = 0;
			if( v > 255 )
				v = 255;
			briConLut[i] = (uchar)v;
		}
	}
	
	cvSetData( briConLutMatrix, briConLut, 0 );

	cvLUT( cvImage, cvImageTemp, briConLutMatrix); 
	swapTemp();
    flagImageChanged();
}
