#ifndef _CALIBMANGER
#define _CALIBMANGER

#include "ofMain.h"

#include "ofxXmlSettings.h"
#include "ofxControlPanel.h"
#include "ofxLeastSquares.h"
#include "calibrationFileSaver.h"

class calibrationManager {

	public:

		void setup();
		void setupControlPanel();
		void update();
		void updateControlPanel();
		void draw();

		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void keyPressed(int key);
	
		void drawLsError();
		void drawRawCalibrationInput(int offsetX, int offsetY, float scale);
		void drawCalibrationData();
		void drawNonCalibration();

		ofRectangle calibrationRectangle;

		void clear();
		void advancePosition();

		void		registerCalibrationInput(float x, float y);
		void		calculateWeights(vector <ofPoint> trackedPoints, vector <ofPoint> knownPoints);		// calculate the weights for an equation that goes from tracked points to known points.

		ofPoint		getCalibratedPoint (float x, float y);
		vector<float> makeOutput(float x, float y);
		vector<float> makeInput(float x, float y);


		vector <ofPoint> screenPoints;
		vector <ofPoint> eyePoints;

		bool		bBeenFit;

		int			nDivisionsWidth;
		int			nDivisionsHeight;
		int			nPosition;

		float		xp, yp;
		int			pos;

		float		inputEnergy;

		void		saveCalibration();
		void		loadCalibration();
		void		calculate();

		bool		bPreAutomatic;

		bool		bAutomatic;
		void		start();
		void		stop();


		bool		bAmInAutodrive;
		float		startTime;
		float		preTimePerDot;
		float		recordTimePerDot;
		float		totalTimePerDot;
		bool		bInAutoRecording;
		bool		bRemovePointsFarFromAverage;

		float		autoPct;

		int			pt;
		int			nPtsPP[25];			// how many points for each point.
		ofPoint		cp[25];				// TODO: replace them with vector
		float		spx[25];			//
		float		spy[25];			//
	

		ofImage					calibrationInfo;
		ofTrueTypeFont			font;

		float					smoothing;
		float removeOutliers, lastRemoveOutliers;
		float					menuEnergy;

		ofxLeastSquares			ls;
	
		int			inputCount;
		int			outputCount;
		
		bool	bDrawCalibrationGrid;
		bool	bDrawLsError;					//least square's error.
		bool	bDrawEyeInput;
		
		bool	bDrawRawInput;
		bool	bDrawRawCalibrationInput;
		bool	bDrawMapOutput;
		ofPoint	rawDataOffset;
		float	rawDataScale;
	
		ofxControlPanel			panel;
	
		calibrationFileSaver calibFileSaver;
		
};

#endif


