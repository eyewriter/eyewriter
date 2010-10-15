#include "glintLineChecker.h"

bool glintLineChecker::lineInRange(const lineSegment& seg) {
//	cout << "seg.distance: " << seg.distance << " minDistance: " << minDistance << " maxDistance: " << maxDistance << endl;
	return seg.distance < minDistance || seg.distance > maxDistance;	
}

bool glintLineChecker::lineCrossGlintInBrightEye(const lineSegment& seg, ofxCvContourFinder & contourFinderBright) {

	int outside = 0;
	
	for (int i = 0; i < contourFinderBright.blobs.size(); i++){
		if (seg.startx > contourFinderBright.blobs[i].centroid.x || seg.endx < contourFinderBright.blobs[i].centroid.x){
			outside++;
		}
	}
	
	if (outside < contourFinderBright.blobs.size()) {
		return false;
	} else {
		return true;
	}
}

void glintLineChecker::setup(int eyeImgWidth, int eyeImgHeight){
	
	width = eyeImgWidth;
	height = eyeImgHeight;
	
	minDistance = 50;
	maxDistance = 200;
	
	myStripesImage.allocate(width, height);
	bDeleteLine = false;
}

void glintLineChecker::update(ofxCvGrayscaleImage & eyeImg, int nGlints, ofxCvContourFinder & contourFinder, bool bUseGlintInBrightEye, ofxCvContourFinder & contourFinderBright){
	
	if (nGlints == 2) {
		
		lineSegments.clear();
		unsigned char * pixels = eyeImg.getPixels();
		
		for (int j = 0; j < eyeImg.height; j++){
			lineSegment temp;
			bool bStarted = false;
			
			for (int i = 0; i < eyeImg.width - 1; i++) {
				
				int pixela = pixels [ j * eyeImg.width + i];
				int pixelb = pixels [ j * eyeImg.width + i + 1];
				
				if ((pixela == 255) && (pixelb == 0)) {
					// yeah!! we are starting !! 
					temp.startx = i;
					temp.starty = j;
					bStarted = true;
				}
				
				if ((pixela == 0) && (pixelb == 255)) {
					
					if (bStarted == true) {
						// cool we are ending :)
						temp.endx = i;
						temp.endy = j;
						temp.distance = temp.endx - temp.startx;
						
						lineSegments.push_back(temp);
						//printf("adding line segment %i %i %i %i -- %i \n", temp.startx, temp.starty, temp.endx, temp.endy, temp.distance );
						
						bStarted = false;
					}
				}
			}
		}
		
		if (bDeleteLine) {
			// remove_if doesn't work now, so for now..
//			lineSegments.erase(remove_if(lineSegments.begin(), lineSegments.end(), glintChecker::lineInRange), lineSegments.end());
			for (int i = 0; i < lineSegments.size(); i++) {
				if (lineInRange(lineSegments[i])) {
					lineSegments.erase(lineSegments.begin() + i);
					i--;
				}
			}
			
			for (int i = 0; i < lineSegments.size(); i++) {
				if (bUseGlintInBrightEye) {
					if (lineCrossGlintInBrightEye(lineSegments[i], contourFinderBright)) {
						lineSegments.erase(lineSegments.begin() + i);
						i--;
					}
				}
			}
		}
		
		cvSetZero(myStripesImage.getCvImage());
		unsigned char * stripepixels = myStripesImage.getPixels();
		
		for (int i = 0; i < lineSegments.size(); i++) {
			int startx = lineSegments[i].startx;
			int endx = lineSegments[i].endx;
			int y = lineSegments[i].starty;
			for (int j = startx; j < endx; j++){
				stripepixels[y * myStripesImage.width + j] = 255;
			}
		}
		myStripesImage.flagImageChanged();
		int nBlobs = linesFinder.findContours(myStripesImage, 100, 10000, 1, false, true);
		
		
		leftGlintID = -1;
		rightGlintID = -1;

		if (nBlobs > 0) {
			ofRectangle foundLinesRect = linesFinder.blobs[0].boundingRect;
			
			for (int i = 0; i < contourFinder.blobs.size(); i++){
				ofRectangle blobRect = contourFinder.blobs[i].boundingRect;
				if (ofInRange(foundLinesRect.x, blobRect.x, blobRect.x + blobRect.width) &&
					(ofInRange(foundLinesRect.y, blobRect.y, blobRect.y + blobRect.height) ||
					 ofInRange(foundLinesRect.y + foundLinesRect.height, blobRect.y, blobRect.y + blobRect.height))){
						leftGlintID = i;
					} else if (ofInRange(foundLinesRect.x + foundLinesRect.width, blobRect.x, blobRect.x + blobRect.width) &&
							   (ofInRange(foundLinesRect.y, blobRect.y, blobRect.y + blobRect.height) ||
								ofInRange(foundLinesRect.y + foundLinesRect.height, blobRect.y, blobRect.y + blobRect.height))) {
						rightGlintID = i;
					}
			}
		}
	}
}

void glintLineChecker::draw(int x, int y){
	
	ofEnableAlphaBlending();
	ofSetColor(255, 0, 0, 80);

	myStripesImage.draw(x, y);
	linesFinder.draw(x,y);

	ofDisableAlphaBlending();
}

