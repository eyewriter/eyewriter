/*
 *  eyeFinder.cpp
 *  EyeTrackDTTest
 *
 *  Created by itotaka on 5/19/10.
 *
 */

#include "eyeFinder.h"

//------------------------------------------------------------------------------------
void eyeFinder::setup(int width, int height, int _targetWidth, int _targetHeight, int divisor){
	
	w = width;
	h = height;
	div = divisor;
	
	wdiv = w / div;
	hdiv = h / div;
	
	previousImg.allocate(wdiv, hdiv);
	diffImg.allocate(wdiv, hdiv);
	currentImg.allocate(wdiv, hdiv);
	
	//	tempCurrent.allocate(wdiv, hdiv);
	//	tempPrevious.allocate(wdiv, hdiv);
	//	temptest_dif.allocate(wdiv, hdiv);
	
	bUseContrast		= false;
	contrast			= 0.2;
	brightness			= 0.0;
	
	bUseDilate			= false;
	nErosions			= 0;
	nDilations			= 1;
	
	bUseBlur			= false;
	blur				= 2;
	
	bUseCompactnessTest =	false;
	maxCompactness = 1.2;
	
	bUseLongShortTest = false;
	minLongShortRatio = 3;
	
	maxWidth = 50000;
	maxHeight = 50000;
	
	minSquareness = 0.5;
	
	targetWidth = _targetWidth;
	targetHeight = _targetHeight;
	
	findingRect.x = (targetWidth/2)/div;
	findingRect.y = (targetHeight/2)/div;
	findingRect.width = wdiv - targetWidth/div;
	findingRect.height = hdiv - targetHeight/div;
	
	bFirstFrame = true;
	bUseGamma = true;
	gamma = 0.57f;
	
}

//------------------------------------------------------------------------------------
bool eyeFinder::update(ofxCvGrayscaleAdvanced & _currentImg, float threshold, float minBlobSize, float maxBlobSize, bool bAllArea = true){
	
	// TODO: change here to get the best one
	
	if (div !=1){
		currentImg.scaleIntoMe(_currentImg, CV_INTER_LINEAR);
	} else {
		currentImg = _currentImg;
	}
	
	if (bFirstFrame){
		previousImg = currentImg;
		bFirstFrame = false;
	}
	
	// Image Filters
	
	// Gamma
	if(bUseGamma){
		currentImg.applyMinMaxGamma(gamma);
	}
	
	if (bUseBlur) {
		currentImg.blur(blur);							//Great!!(also for subtraction!!!)
	}
	
	if (bUseContrast){
		currentImg.applyBrightnessContrast(brightness,contrast);
	}
	
	// get diff Image;		
	diffImg.absDiff(currentImg, previousImg);
	diffImg.threshold(threshold, false);
	
	if (bUseDilate){
		
		int maxOperations		= MAX(nDilations, nErosions);
		
		for (int i = 0; i < maxOperations; i++) {
			
			if (i < nErosions)	diffImg.erode();
			if (i < nDilations)	diffImg.dilate();
			
		}
	}
	
	bFoundOne = false;
	bBadFrame = false;
	
	if (diffImg.countNonZeroInRegion(0,0,diffImg.width, diffImg.height) > 80000 / (div * div)) bBadFrame = true;		// not sure.. are there some faster way?
	
	if (!bBadFrame) {
		
		offsetROI.x = findingRect.x * div;
		offsetROI.y = findingRect.y * div;
		offsetROI.width = findingRect.width * div;
		offsetROI.height = findingRect.height * div;
		
		diffImg.setROI(findingRect);
		int num = contourFinder.findContours(diffImg, minBlobSize, maxBlobSize, 20, true, true);
		
		// a bug is in the "findContours" in ofxCvContourFinder.
		// I changed the order of lines.
		// see findContour in ofxCvContourFinder.		
		//	
		// To use findContour with ROI, I changed the codes like below.
		//
		// From:
		// inputCopy = input;
		// inputCopy.setROI( input.getROI() );
		//
		// To:
		// inputCopy.setROI( input.getROI() );
		// inputCopy = input;
		
		diffImg.resetROI();
		
		if( num ){
			
			for(int k = 0; k < num; k++){
				
				// compactness check
				float arcl = contourFinder.blobs[k].length;
				float area = contourFinder.blobs[k].area;
				
				float compactness;
				
				if (bUseCompactnessTest){
					compactness = (float)((arcl*arcl/area)/FOUR_PI);		//(((2PI * r)^2 / PI * r^2) / 4PI) = 1 : compactness of circle. maybe minimum?
					if (compactness > maxCompactness) continue;
				}
				
				
				// ratio check with rotated bounding rect
				float longshortratio;
				if (bUseLongShortTest){
					getRotatedBoundingBox(k);
					longshortratio =	box.size.width < box.size.height ? box.size.width / box.size.height : box.size.height / box.size.width;
					if (longshortratio < minLongShortRatio) continue;
				}
				
				
				// bounding rect width & height test.
				if (contourFinder.blobs[k].boundingRect.width > maxWidth/div || contourFinder.blobs[k].boundingRect.height > maxHeight/div) {
					continue;
				}
				
				
				//lets ignore rectangular blobs
				float ratio =	contourFinder.blobs[k].boundingRect.width < contourFinder.blobs[k].boundingRect.height ?
				contourFinder.blobs[k].boundingRect.width / contourFinder.blobs[k].boundingRect.height :
				contourFinder.blobs[k].boundingRect.height / contourFinder.blobs[k].boundingRect.width;
				
				if( ratio > minSquareness){
					
					bFoundOne = true;
					
					boundingRect.x = contourFinder.blobs[k].boundingRect.x * div + offsetROI.x;
					boundingRect.y = contourFinder.blobs[k].boundingRect.y * div + offsetROI.y; 
					boundingRect.width = contourFinder.blobs[k].boundingRect.width * div;
					boundingRect.height = contourFinder.blobs[k].boundingRect.height * div;
					
					centroid = (contourFinder.blobs[k].centroid + ofPoint(findingRect.x, findingRect.y)) * div;
					foundBlob = contourFinder.blobs[k];
					
					// !!!: only to check
					//					tempCurrent = currentImg;
					//					tempPrevious = previousImg;
					//					temptest_dif = diffImg;
					
					if (!bAllArea){
						
						findingRect.x = fabs(boundingRect.x - 150) / div;
						findingRect.y = fabs(boundingRect.y - 100) / div;
						findingRect.width = CLAMP(boundingRect.width + 300, 0, w - boundingRect.x - boundingRect.width) / div;
						findingRect.height = CLAMP(boundingRect.height + 200, 0, h - boundingRect.y - boundingRect.height) / div;
						
					} else {
						
						findingRect.x = (targetWidth/2)/div;
						findingRect.y = (targetHeight/2)/div;
						findingRect.width = wdiv - targetWidth/div;
						findingRect.height = hdiv - targetHeight/div;
					}
					
					break;
				}
			}
		}
	}	
	
	previousImg = currentImg;
	
	return bFoundOne;
}

//------------------------------------------------------------------------------------
void eyeFinder::drawFindingRect(float x, float y, float width, float height){
	
	ofRect(findingRect.x * div * width/w + x, findingRect.y * div * height/h + y, findingRect.width * div * width/w, findingRect.height * div * height/h);
	
}

//------------------------------------------------------------------------------------
void eyeFinder::getRotatedBoundingBox(int blobNum){
	
	CvMemStorage	* m_storage = cvCreateMemStorage(0);
	CvSeq* output = cvCreateSeq( CV_SEQ_KIND_GENERIC|CV_32SC2,sizeof(CvContour),sizeof(CvPoint),m_storage);
	for( int i=0; i< contourFinder.blobs[blobNum].pts.size(); i++) {
		CvPoint pt;
		pt.x = contourFinder.blobs[blobNum].pts[i].x;
		pt.y = contourFinder.blobs[blobNum].pts[i].y;
		cvSeqPush( output, &pt );
	}
	CvRect rect	= cvBoundingRect( output, 1 );
	box = cvMinAreaRect2(output, NULL );
	
	cvClearSeq(output);
	cvReleaseMemStorage( &m_storage );
	
}

//------------------------------------------------------------------------------------

