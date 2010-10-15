#include "eyeFinder.h"

//------------------------------------------------------------------------------------
void eyeFinder::setup(int width, int height, int _targetWidth, int _targetHeight, int divisor){
	
	w = width;					// width from inputManager (640 normally)
	h = height;					// height from inputManager (480 normally)
	div = divisor;				// divisor to get smaller image (default 2)
	
	wdiv = w / div;
	hdiv = h / div;
	
	previousImg.allocate(wdiv, hdiv);			// TODO: use array of images here, and then can remove one =(copy) process.
	diffImg.allocate(wdiv, hdiv);
	currentImg.allocate(wdiv, hdiv);			// TODO: use array of images here, and then can remove one =(copy) process.
		
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
	
	currentImg = _currentImg;			// _currentImg is already small(width&height are devided by div) from eyeTracker.
	
	if (bFirstFrame){
		previousImg = currentImg;			// this is slow a bit. can do the same thing wighout this copy. just alternate the valuable every frame.
		bFirstFrame = false;
	}
	
	// Image Filters
	
	if (bUseGamma) currentImg.applyMinMaxGamma(gamma);
	if (bUseBlur) currentImg.blur(blur);							//Great!!(also for subtraction!!!)
	if (bUseContrast) currentImg.applyBrightnessContrast(brightness,contrast);
	
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
				
				if( ratio > minSquareness) {
					
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
					
					break;
				}
			}
		}
		
		if (!bAllArea){
			
			if (bFoundOne){
				findingRect.x = fabs(centroid.x - targetWidth) / div;
				findingRect.y = fabs(centroid.y - targetHeight) / div;
				findingRect.width =  targetWidth * 2 / div;
				findingRect.height = targetHeight * 2 / div;
				
			} else {
				
				findingRect.x = (targetWidth/2)/div;
				findingRect.y = (targetHeight/2)/div;
				findingRect.width = wdiv - targetWidth/div;
				findingRect.height = hdiv - targetHeight/div;
			}
			
		}
		// if (bAllArea), we don't have to change findingRect after setup.
		
	}	
	
	previousImg = currentImg;
	
	return bFoundOne;
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
void eyeFinder::draw(float x, float y, float width, float height){
	
	ofNoFill();
	
	ofEnableAlphaBlending();
	ofSetColor(255,255,255, 80);
	
	currentImg.draw(x, y, width, height);
	diffImg.draw(x, y, width, height);
	
	ofNoFill();
	
	ofSetColor(255,0,0, 130);
	ofRect(x + boundingRect.x * width / w, y + boundingRect.y * height / h,
		   boundingRect.width * width / w, boundingRect.height * height / h);
	
	float	targetRectTempX = centroid.x - (targetWidth/2);
	float	targetRectTempY = centroid.y - (targetHeight/2);

	if (bFoundOne){
		ofSetColor(0,0,255, 150);
		ofRect(x + targetRectTempX * width / w, y + targetRectTempY * height / h,
			   targetWidth  * width / w, targetHeight * height / h);
	}
		
	ofSetColor(255, 0, 0,100);
	ofRect(findingRect.x * div * width/w + x, findingRect.y * div * height/h + y, findingRect.width * div * width/w, findingRect.height * div * height/h);
	
	if (bFoundOne){
		ofSetColor(255,255,255);
		foundBlob.draw(x + offsetROI.x * width / w, y + offsetROI.y * height / h);
	}
		
	ofDisableAlphaBlending();
	
	ofSetColor(255, 255, 255);
	ofDrawBitmapString("EyeFinder", x + 1, y + height + 12);
	
}

//------------------------------------------------------------------------------------

