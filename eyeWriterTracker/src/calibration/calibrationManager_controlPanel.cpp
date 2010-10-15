#include "calibrationManager.h"

void calibrationManager::setupControlPanel(){

	panel.setup("calibration", 1080, 10, 270, 500);
	panel.addPanel("auto calibration", 1, false);
	panel.addPanel("display calibration data", 1, false);
	panel.addPanel("calibration setup", 1, false);
	panel.addPanel("manual calibration", 1, false);
	panel.addPanel("smoothing", 1, false);

	
	//---- gaze
	panel.setWhichPanel("calibration setup");
	panel.addSlider("num divisions horiz", "N_DIV_W", nDivisionsWidth, 2, 15, true);
	panel.addSlider("num divisions vert", "N_DIV_H", nDivisionsHeight, 2, 15, true);

	panel.addSlider("(auto) time to pre-record", "PRE_RECORD_TIME", preTimePerDot, 0.1, 5, false);
	panel.addSlider("(auto) time for record", "RECORD_TIME", recordTimePerDot, 0.1, 1.5, false);

	panel.addToggle("remove points far from each average", "B_REMOVE_FAR_AVERAGE", false);
	panel.addSlider("threshold distance(multiplier to avrage)", "THRESHOLD_DIST", 1.7, 0, 4, false);

	panel.setWhichPanel("auto calibration");

	panel.addToggle("start auto calibration", "START_AUTO", false);
	panel.addToggle("reset calibration", "RESET_CALIB", false);
	panel.addToggle("save calibration", "SAVE_CALIB", false);
	panel.addToggle("load calibration", "LOAD_CALIB", false);
	
	panel.addToggle("mouse simulation", "B_MOUSE_SIMULATION", false);

	panel.loadSettings("settings/calibrationSettings.xml");


	panel.setWhichPanel("display calibration data");

	panel.addToggle("bDrawRawInput", "B_DRAW_RAWINPUT", false);
	panel.addToggle("bDrawRawCalibrationInput", "B_DRAW_CALIBINPUT", false);
	panel.addToggle("bDrawMapOutput", "B_DRAW_MAPOUTPUT", false);
	panel.addToggle("bDrawLsError", "B_DRAW_LS_ERROR", true);
	panel.addToggle("bDrawEyeInput", "B_DRAW_EYE_INPUT", true);

	panel.addSlider("RawDateScale", "RAW_SCALE", 20, 10, 100, false);
	panel.addSlider("RawDataOffsetX", "RAW_OFFSET_X", ofGetWidth()/2, 0, ofGetWidth()*2, true);
	panel.addSlider("RawDataOffsetY", "RAW_OFFSET_Y", ofGetHeight()/2, 0, ofGetHeight()*2, true);


	panel.setWhichPanel("smoothing");
	panel.addSlider("remove outliers", "REMOVE_OUTLIERS", 2, .1, 4, false);
	panel.addSlider("smoothing amount", "AMOUNT_SMOOTHING", 0.993, 0.8, 1.0f, false);
}

//--------------------------------------------------------------
void calibrationManager::updateControlPanel(){
	
	panel.update();

	preTimePerDot = panel.getValueF("PRE_RECORD_TIME");;
	recordTimePerDot = panel.getValueF("RECORD_TIME");;
	smoothing = panel.getValueF("AMOUNT_SMOOTHING");
	
	fitter.removeOutliersf = panel.getValueF("REMOVE_OUTLIERS");
	
	if (panel.getValueB("START_AUTO")){
		bPreAutomatic = true;
		panel.setValueB("START_AUTO", false);
	}
	
	if (panel.getValueB("RESET_CALIB")){
		clear();
		panel.setValueB("RESET_CALIB", false);
	}
	
	if (panel.getValueB("SAVE_CALIB")){
		if (fitter.bBeenFit){
			fitter.saveCalibration();
		}
		panel.setValueB("SAVE_CALIB", false);
	}
	
	if (panel.getValueB("LOAD_CALIB")){
		fitter.loadCalibration();
		panel.setValueB("LOAD_CALIB", false);
	}
	
	fitter.bDrawRawCalibrationInput = panel.getValueB("B_DRAW_CALIBINPUT");
	fitter.bDrawLsError = panel.getValueB("B_DRAW_LS_ERROR");
	bDrawMapOutput = panel.getValueB("B_DRAW_MAPOUTPUT");
	bDrawRawInput = panel.getValueB("B_DRAW_RAWINPUT");
	bDrawEyeInput = panel.getValueB("B_DRAW_EYE_INPUT");
	
	fitter.rawDataScale = panel.getValueF("RAW_SCALE");
	fitter.rawDataOffset.x = panel.getValueF("RAW_OFFSET_X");
	fitter.rawDataOffset.y = panel.getValueF("RAW_OFFSET_Y");
	
//	bMouseSimulation = panel.getValueB("B_MOUSE_SIMULATION");
	
}

