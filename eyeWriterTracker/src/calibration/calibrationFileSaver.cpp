/*
 *  calibrationFileSaver.cpp
 *  RemoteEyeTracker
 *
 *  Created by Takayuki Ito on 7/9/10.
 *  Copyright 2010 -. All rights reserved.
 *
 */

#include "calibrationFileSaver.h"

//--------------------------------------------------------------
void calibrationFileSaver::saveCalibration(ofxLeastSquares& ls, int nRefPoints, float spx[], float spy[], ofPoint cp[], vector<ofPoint>& eyePoints, vector<ofPoint>& screenPoints){
	
	ofxXmlSettings xml;
	vector< vector<float> > map = ls.getMap();
	for (int i = 0; i < map.size(); i++){
		xml.addTag("term");
		xml.pushTag("term", i);
		vector<float>& cur = map[i];
		xml.addValue("x", cur[0]);
		xml.addValue("y", cur[1]);
		xml.popTag();
	}
	
	xml.addValue("nRefPoints", nRefPoints);
	
	for (int i = 0; i < nRefPoints; i++) {
		xml.addTag("referencepoint");
		xml.pushTag("referencepoint", i);
		xml.addValue("x", spx[i]);
		xml.addValue("y", spy[i]);
		xml.popTag();
		
		xml.addTag("inputaverage");
		xml.pushTag("inputaverage", i);
		xml.addValue("x", cp[i].x);
		xml.addValue("y", cp[i].y);
		xml.popTag();
	}
	
	cout << "eyePoints: " << eyePoints.size() << endl;
	xml.addValue("nEyeRawInputPoints", (int)eyePoints.size());
	
	for (int i = 0; i < eyePoints.size(); i++) {
		xml.addTag("eyePoints");
		xml.pushTag("eyePoints", i);
		xml.addValue("x", eyePoints[i].x);
		xml.addValue("y", eyePoints[i].y);
		xml.popTag();
	}
	
	xml.addValue("nScreenRefPoints", (int)screenPoints.size());
	
	for (int i = 0; i < eyePoints.size(); i++) {
		xml.addTag("screenpoints");
		xml.pushTag("screenpoints", i);
		xml.addValue("x", screenPoints[i].x);
		xml.addValue("y", screenPoints[i].y);
		xml.popTag();
	}
	
	xml.saveFile("settings/calibration.xml");
	
}

//--------------------------------------------------------------
void calibrationFileSaver::loadCalibration(ofxLeastSquares& ls, float spx[], float spy[], ofPoint cp[], vector<ofPoint>& eyePoints, vector<ofPoint>& screenPoints){
			
	eyePoints.clear();
	screenPoints.clear();
	
	ofxXmlSettings xml;
	xml.loadFile("settings/calibration.xml");
	vector< vector<float> > map;
	
	for (int i = 0; i < 6; i++) { // ideally this shouldn't be fixed at 6
		xml.pushTag("term", i);
		vector<float> cur;
		cur.push_back(xml.getValue("x", 0.0f ));
		cur.push_back(xml.getValue("y", 0.0f ));
		map.push_back(cur);
		xml.popTag();
	}
	
	for (int i = 0; i < xml.getValue("nRefPoints", 0); i++) {
		
		xml.pushTag("referencepoint",i);
		spx[i] = xml.getValue("x", 0.0f);
		spy[i] = xml.getValue("y", 0.0f);
		xml.popTag();
		
		xml.pushTag("inputaverage", i);
		cp[i].x = xml.getValue("x", 0.0f);
		cp[i].y = xml.getValue("y", 0.0f);
		xml.popTag();
	}
	
	for (int i = 0; i < xml.getValue("nEyeRawInputPoints", 0); i++) {
		xml.pushTag("eyePoints", i);
		eyePoints.push_back(ofPoint(xml.getValue("x", 0.0f ), xml.getValue("y", 0.0f)));
		xml.popTag();
	}
	
	for (int i = 0; i < xml.getValue("nScreenRefPoints", 0); i++){
		xml.pushTag("screenpoints", i);
		screenPoints.push_back(ofPoint(xml.getValue("x", 0.0f ), xml.getValue("y", 0.0f)));
		xml.popTag();
	}

	ls.setMap(map);

}
//--------------------------------------------------------------

