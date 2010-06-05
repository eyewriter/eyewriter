#ifndef _CALIBMANGER
#define _CALIBMANGER


#include "ofMain.h"

#include "ofxXmlSettings.h"
#include "ofxControlPanel.h"
#include "ofxLeastSquares.h"


class calibrationManager {

	public:
	
		
		void setup();
		void update();
		void draw();
	
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void keyPressed(int key);
	
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
		
		float		autoPct;
		
		ofImage					calibrationInfo;
		ofTrueTypeFont			font;
	
		float					smoothing;
		float					menuEnergy;
	
		ofxControlPanel			panel;
	
		ofxLeastSquares			ls;
	int						inputCount;
	int						outputCount;
	
	
};

#endif
