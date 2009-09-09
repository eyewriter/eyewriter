#ifndef _CALIBMANGER
#define _CALIBMANGER


#include "ofMain.h"

#include "ofxXmlSettings.h"


class calibrator {

	public:
		void setup();
		void update();
		void draw();
	
	
		void		clear();
		void		advancePosition();
		
		void		registerCalibrationInput(float x, float y);
		void		calculateWeights(vector <ofPoint> eyePoints, vector <ofPoint>  screenPoints);
	
		ofPoint		getCalibratedPoint (float x, float y);

		vector <ofPoint> follow;
		vector <ofPoint> myMouse;
	
		double		cxfit[6];
		double		cyfit[6];
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
		
		bool		bAutomatic;
		void		start();
		bool		bAmInAutodrive;
		float		startTime;
		float		preTimePerDot;
		float		recordTimePerDot;
		float		totalTimePerDot;
		bool		bInAutoRecording;
		
		float		autoPct;
	
	
	
};

#endif
