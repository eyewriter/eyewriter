#ifndef _THREADED_IMG_SAVER
#define _THREADED_IMG_SAVER

#include "ofMain.h"
#include "ofxThread.h"

class threadedImageSaver : public ofxThread{

	public:

		unsigned char * pixelData;
		string name;

		//--------------------------
		threadedImageSaver(unsigned char * pixels, string myName){
//			pixelData = new unsigned char [640*480];
//			memcpy(pixelData, pixels, 640*480);
			pixelData = new unsigned char [640*480*3];
			memcpy(pixelData, pixels, 640*480*3);
			name = myName;
		}

		void start(){
            startThread(true, false);   // blocking, verbose
        }

        void stop(){
            stopThread();
        }

		//--------------------------
		void threadedFunction(){
			ofImage temp;
			temp.setUseTexture(false);
//			temp.allocate(640, 480, OF_IMAGE_GRAYSCALE);
//			temp.setFromPixels(pixelData, 640, 480, OF_IMAGE_GRAYSCALE);
			temp.allocate(640, 480, OF_IMAGE_COLOR);
			temp.setFromPixels(pixelData, 640, 480, OF_IMAGE_COLOR);
			temp.saveImage(name);
			delete [] pixelData;
			stop();
		}
};

#endif /* _THREADED_IMG_SAVER */
