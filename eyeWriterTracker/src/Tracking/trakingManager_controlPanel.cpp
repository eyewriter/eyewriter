#include "trackingManager.h"

// TODO: Reorganize here well.

//--------------------------------------------------------------
void trackingManager::setupGui(){
	
	panel.setup("cv panel", 1080, 10, 270, 820);
	panel.addPanel("image adjustment", 1, false);
	//	panel.addPanel("edge fixer", 1, false);
	panel.addPanel("eye detection 1", 1, false);
	panel.addPanel("eye detection 2", 1, false);
	
	panel.addPanel("pupil detection 1", 1, false);
	panel.addPanel("pupil detection 2", 1, false);
	panel.addPanel("glint detection", 1, false);
	panel.addPanel("misc", 1, false);
	
	
	//---- image ---
	panel.setWhichPanel("image adjustment");
	panel.setWhichColumn(0);
	
	if (IM.mode == INPUT_VIDEO){
		panel.addSlider("video speed", "VIDEOSPEED", 1, 0, 2, false);
		// TODO: add theo's video playing things.... [zach]
	} else {
		panel.addToggle("load video settings", "VIDEO_SETTINGS", false);
	}
	
	panel.addToggle("Focus Screen", "B_FOCUS_SCREEN", false);
	
	panel.addSlider("threshold(eye)", "THRESHOLD_EYEPOS", 60, 0, 255, true);
	panel.addSlider("threshold(pupil)", "THRESHOLD_PUPIL", 60, 0, 255, true);
	panel.addSlider("threshold(glint)", "THRESHOLD_GLINT", 190, 0, 255, true);
	panel.addToggle("auto threshold(pupil)", "B_USE_AUTOTHRESHOLD_PUPIL", true);
	panel.addToggle("auto threshold(glint)", "B_USE_AUTO_THRESHOLD_GLINT", true);
	
	panel.addToggle("record reference glints", "B_RECORD_GLINTS", false);
	panel.addToggle("delete wrong lines", "B_DELETE_LINES", true);
	panel.addSlider("min distance", "GLINT_MIN_DISTANCE", 60, 0, 300, false);
	panel.addSlider("max distance", "GLINT_MAX_DISTANCE", 200, 0, 400, false);
	panel.addToggle("use recorded glint ref", "B_USE_RECORDED_GRINTREF", true);
	panel.addSlider("min distance(pct to ref)", "GLINT_MIN_DIS_PCT", 0.75, 0, 1, false);
	panel.addSlider("max distance(pct to ref)", "GLINT_MAX_DIS_PCT", 1.5, 1, 20, false);
	
	panel.addToggle("use glint in bright eye", "B_BRIGHT_GLINT", true);
	

	
	if(IM.mode == INPUT_LIVE_VIDEO && IM.grabberType == INPUT_OFXLIBDC) {
		ofxLibdcPtGrey& cam = *((ofxLibdcPtGrey*) IM.vidGrabber);
		lastShutter = cam.getShutterNorm();
		lastGain = cam.getGainNorm();
		lastExposure = cam.getExposureNorm();
		lastGamma = cam.getGammaNorm();
		lastBrightness = cam.getBrightnessNorm();
		panel.addSlider("shutter", "INPUT_SHUTTER", lastShutter, 0, 1, false);
		panel.addSlider("gain", "INPUT_GAIN", lastGain, 0, 1, false);
		panel.addSlider("exposure", "INPUT_EXPOSURE", lastExposure, 0, 1, false);
		panel.addSlider("gamma", "INPUT_GAMMA", lastGamma, 0, 1, false);
		panel.addSlider("brightness", "INPUT_BRIGHTNESS", lastBrightness, 0, 1, false);
	}
	
	// eye positon
	
	panel.setWhichPanel("eye detection 1");
	
	panel.addSlider("min blob(eye)","MIN_BLOB_EYEPOS",50*10,0,5000,true);
	panel.addSlider("max blob(eye)","MAX_BLOB_EYEPOS",100*100,0,50500,true);
	
	panel.addSlider("maxblobWidth", "MAX_BLOB_WIDTH_EYEPOS", 300, 0, 500, false);
	panel.addSlider("maxblobHeight", "MAX_BLOB_HEIGHT_EYEPOS", 300, 0, 500, false);
	
	panel.addToggle("use compactness", "B_USE_COMPACTNESS_EYEPOS", true);
	panel.addSlider("MaxCompactness", "MAX_COMPACTNESS_EYEPOS", 2.0, 1, 3, false);
	panel.addToggle("use long short ratio", "B_USE_LONG_SHORT_EYEPOS", false);
	panel.addSlider("Long_Short_Ratio", "RATIO_LONG_SHORT_EYEPOS", 0.4, 0, 1, false);
	
	panel.addSlider("minSquareness", "MIN_SQUARENESS_EYEPOS", 0.5, 0, 1, false);
	
	
	
	panel.setWhichPanel("eye detection 2");
	
	panel.addToggle("use erode/dilate(eye)", "B_USE_DILATE_EYEPOS", true);
	panel.addSlider("erode num(eye)", "N_EROSIONS_EYEPOS", 0, 0, 20, true);
	panel.addSlider("dilate num(eye)", "N_DILATIONS_EYEPOS", 0, 0, 20, true);
	
	panel.addToggle("use gamma ", "B_USE_GAMMA_EYEPOS", true);
	panel.addSlider("gamma ", "GAMMA_EYEPOS", 0.57f, 0.01, 3.0f, false);
	
	panel.addToggle("use contrast / bri", "B_USE_CONTRAST_EYEPOS", false);
	panel.addSlider("contrast ", "CONTRAST_EYEPOS", 0.28f, 0.0, 1.0f, false);
	panel.addSlider("brightness ", "BRIGHTNESS_EYEPOS", -0.02f, -1.0, 3.0f, false);
	
	panel.addToggle("use blur", "B_USE_BLUR_EYEPOS", true);
	panel.addSlider("blur ", "BLUR_EYEPOS", 5, 0, 20, false);
		
	
	
	// pupil position 1
	panel.setWhichPanel("pupil detection 1");
	
	panel.addSlider("min blob(pupil)","MIN_BLOB_PUPIL",50*10,0,5000,true);
	panel.addSlider("max blob(pupil)","MAX_BLOB_PUPIL",100*100,0,50500,true);
	
	panel.addSlider("maxblobWidth", "MAX_BLOB_WIDTH_PUPIL", 300, 0, 500, false);
	panel.addSlider("maxblobHeight", "MAX_BLOB_HEIGHT_PUPIL", 300, 0, 500, false);
	
	panel.addToggle("use erode/dilate(pupil)", "B_USE_DILATE_PUPIL", true);
	panel.addSlider("erode num", "N_EROSIONS_PUPIL", 0, 0, 20, true);
	panel.addSlider("dilate num ", "N_DILATIONS_PUPIL", 0, 0, 20, true);
	
	panel.addToggle("use blur", "B_USE_BLUR_PUPIL", true);
	panel.addSlider("blur ", "BLUR_PUPIL", 5, 0, 20, false);
	
	panel.addToggle("use contrast stretch", "B_USE_STRETCH_PUPIL", false);
	
	
	
	// pupil position 2
	panel.setWhichPanel("pupil detection 2");

	panel.addSlider("scan X", "SCAN_X_PUPIL", 0, 0, 287, true);
	panel.addSlider("scan Y", "SCAN_Y_PUPIL", 0, 0, 255, true);
	
	panel.addToggle("use gamma ", "B_USE_GAMMA_PUPIL", true);
	panel.addSlider("gamma ", "GAMMA_PUPIL", 0.57f, 0.01, 3.0f, false);
	
	panel.addToggle("use contrast / bri", "B_USE_CONTRAST_PUPIL", false);
	panel.addSlider("contrast ", "CONTRAST_PUPIL", 0.28f, 0.0, 1.0f, false);
	panel.addSlider("brightness ", "BRIGHTNESS_PUPIL", -0.02f, -1.0, 3.0f, false);
		
	
	
	// glint position
	panel.setWhichPanel("glint detection");
	
	panel.addSlider("w/h limiting ratio", "WH_RATIO", 0.7, 0.1, 1, false);
	
	panel.addSlider("min blob(glint)","MIN_BLOB_GLINT",5,0,2000,true);
	panel.addSlider("max blob(glint)","MAX_BLOB_GLINT",500,0,20000,true);
	
	panel.addToggle("draw line(pupil)", "B_DRAW_LINE_P", true);
	panel.addToggle("draw line(glint)", "B_DRAW_LINE_G", true);
	panel.addToggle("draw contour", "B_DRAW_CONTOUR", false);
	
	panel.addToggle("Use contrast stretch ", "B_CONTRAST_STRETCH", true);
	panel.addSlider("range Min", "RANGE_MIN_GLINT", 0, -500, 500, false);
	panel.addSlider("range Max", "RANGE_MAX_GLINT", 255, -255, 1000, false);
	
	panel.addSlider("ROIorigin(x)", "ROI_ORIGIN_X", 0, 0, 1, false);
	panel.addSlider("ROIorigin(y)", "ROI_ORIGIN_Y", 0, 0, 1, false);
	panel.addSlider("ROIend(x)", "ROI_END_X", 1, 0, 1, false);
	panel.addSlider("ROIend(y)", "ROI_END_Y", 1, 0, 1, false);
	
	
	
	panel.setWhichPanel("misc");
	panel.addToggle("draw raw input", "B_DRAW_RAW_INPUT", false);
	
	panel.addToggle("flip horizontal ", "B_RIGHT_FLIP_X", false);
	panel.addToggle("flip vertical ", "B_RIGHT_FLIP_Y", false);
	
	panel.addToggle("num of glints(0: 2, 1: 4)", "B_FOUR_GLINTS", true);
	panel.addToggle("Use Homography", "B_USE_HOMOGRAPHY", false);
	

	panel.loadSettings("settings/trackingSettings.xml");
	
}

//--------------------------------------------------------------
void trackingManager::updateGui(){
	
	// image adjustment
	
	IM.flipX = panel.getValueB("B_RIGHT_FLIP_X");
	IM.flipY = panel.getValueB("B_RIGHT_FLIP_Y");
	
	bFocusScreenMode = panel.getValueB("B_FOCUS_SCREEN");
	
	// check to see if the camera parameters have been adjusted
	// if so, apply them to the camera
	if(IM.mode == INPUT_LIVE_VIDEO && IM.grabberType == INPUT_OFXLIBDC) {
		float curShutter = panel.getValueF("INPUT_SHUTTER");
		float curGain = panel.getValueF("INPUT_GAIN");
		float curBrightness = panel.getValueB("INPUT_BRIGHTNESS");
		float curExposure = panel.getValueF("INPUT_EXPOSURE");
		float curGamma = panel.getValueF("INPUT_GAMMA");
		
//		ofxLibdcPtGrey& cam = *((ofxLibdcPtGrey*) IM.vidGrabber);
		fireflyVideoGrabber& cam = *((fireflyVideoGrabber*) IM.vidGrabber);
		
		if(curShutter != lastShutter)
			cam.setShutterNorm(curShutter);
		if(curGain != lastGain)
			cam.setGainNorm(curGain);
		if(curBrightness != lastBrightness)
			cam.setBrightnessNorm(curBrightness);
		if(curExposure != lastExposure)
			cam.setExposureNorm(curExposure);
		if(curGamma != lastGamma)
			cam.setGammaNorm(curGamma);
		
		lastShutter = curShutter;
		lastGain = curGain;
		lastBrightness = curBrightness;
		lastExposure = curExposure;
		lastGamma = curGamma;
	}
	
	bDrawRawInput = panel.getValueB("B_DRAW_RAW_INPUT");
	
	
	// eye position
	
	tracker.eFinder.bUseDilate		= panel.getValueB("B_USE_DILATE_EYEPOS");
	tracker.eFinder.nErosions		= panel.getValueF("N_EROSIONS_EYEPOS");
	tracker.eFinder.nDilations		= panel.getValueF("N_DILATIONS_EYEPOS");
	
	tracker.eFinder.bUseGamma		= panel.getValueB("B_USE_GAMMA_EYEPOS");
	tracker.eFinder.gamma			= panel.getValueF("GAMMA_EYEPOS");
	
	tracker.eFinder.bUseContrast	= panel.getValueB("B_USE_CONTRAST_EYEPOS");
	tracker.eFinder.contrast		= panel.getValueF("CONTRAST_EYEPOS");
	tracker.eFinder.brightness		= panel.getValueF("BRIGHTNESS_EYEPOS");
	
	tracker.eFinder.bUseBlur		= panel.getValueB("B_USE_BLUR_EYEPOS");
	tracker.eFinder.blur			= panel.getValueF("BLUR_EYEPOS");
	
	tracker.threshold_e				= panel.getValueF("THRESHOLD_EYEPOS");
	tracker.minBlobSize_e			= panel.getValueI("MIN_BLOB_EYEPOS");
	tracker.maxBlobSize_e			= panel.getValueI("MAX_BLOB_EYEPOS");
	
	tracker.eFinder.maxWidth		= panel.getValueF("MAX_BLOB_WIDTH_EYEPOS");
	tracker.eFinder.maxHeight		= panel.getValueF("MAX_BLOB_HEIGHT_EYEPOS");
	
	tracker.eFinder.bUseCompactnessTest = panel.getValueB("B_USE_COMPACTNESS_EYEPOS");
	tracker.eFinder.maxCompactness	= panel.getValueF("MAX_COMPACTNESS_EYEPOS");
	
	tracker.eFinder.bUseLongShortTest = panel.getValueB("B_USE_LONG_SHORT_EYEPOS");
	tracker.eFinder.minLongShortRatio = panel.getValueF("RATIO_LONG_SHORT_EYEPOS");
	
	tracker.eFinder.minSquareness	= panel.getValueF("MIN_SQUARENESS_EYEPOS");
	
	
	// Pupil Position
	
	tracker.pFinder.bUseDilate		= panel.getValueB("B_USE_DILATE_PUPIL");
	tracker.pFinder.nErosions		= panel.getValueF("N_EROSIONS_PUPIL");
	tracker.pFinder.nDilations		= panel.getValueF("N_DILATIONS_PUPIL");
	
	tracker.pFinder.bUseGamma		= panel.getValueB("B_USE_GAMMA_PUPIL");
	tracker.pFinder.gamma			= panel.getValueF("GAMMA_PUPIL");
	
	tracker.pFinder.bUseContrast	= panel.getValueB("B_USE_CONTRAST_PUPIL");
	tracker.pFinder.contrast		= panel.getValueF("CONTRAST_PUPIL");
	tracker.pFinder.brightness		= panel.getValueF("BRIGHTNESS_PUPIL");
	
	tracker.pFinder.bUseBlur		= panel.getValueB("B_USE_BLUR_PUPIL");
	tracker.pFinder.blur			= panel.getValueF("BLUR_PUPIL");
		
	tracker.bUseAutoThreshold_p		= panel.getValueB("B_USE_AUTOTHRESHOLD_PUPIL");
	tracker.threshold_p_frompanel	= panel.getValueF("THRESHOLD_PUPIL");
	tracker.minBlobSize_p			= panel.getValueF("MIN_BLOB_PUPIL");
	tracker.maxBlobSize_p			= panel.getValueF("MAX_BLOB_PUPIL");
	
	tracker.pFinder.maxWidth		= panel.getValueF("MAX_BLOB_WIDTH_PUPIL");
	tracker.pFinder.maxHeight		= panel.getValueF("MAX_BLOB_HEIGHT_PUPIL");
	
	tracker.pFinder.bUseContrastStretch = panel.getValueB("B_USE_STRETCH_PUPIL");

	
	// Pupil Position 2

	tracker.thresCal.scanX = panel.getValueF("SCAN_X_PUPIL");
	tracker.thresCal.scanY = panel.getValueF("SCAN_Y_PUPIL");
	
	
	// Glint Position
	
	tracker.gFinder.bFourGlints		= panel.getValueB("B_FOUR_GLINTS");
	tracker.bUseHomography	= panel.getValueB("B_USE_HOMOGRAPHY");
	
//	tracker.gFinder.gChecker.limitratio = panel.getValueF("WH_RATIO");
	
	tracker.bUseAutoThreshold_g		= panel.getValueB("B_USE_AUTO_THRESHOLD_GLINT");
	tracker.threshold_g_frompanel	= panel.getValueF("THRESHOLD_GLINT");
	tracker.minBlobSize_g			= panel.getValueF("MIN_BLOB_GLINT");
	tracker.maxBlobSize_g			= panel.getValueF("MAX_BLOB_GLINT");
	
	tracker.gFinder.pctGlintROIorigin.x = panel.getValueF("ROI_ORIGIN_X");
	tracker.gFinder.pctGlintROIorigin.y = panel.getValueF("ROI_ORIGIN_Y");
	tracker.gFinder.pctGlintROIend.x = panel.getValueF("ROI_END_X");
	tracker.gFinder.pctGlintROIend.y = panel.getValueF("ROI_END_Y");
	
	tracker.gFinder.bUseContrastStretch = panel.getValueB("B_CONTRAST_STRETCH");
	
	tracker.gFinder.bRecordRef = panel.getValueB("B_RECORD_GLINTS");
	tracker.gFinder.gLineChecker.bDeleteLine = panel.getValueB("B_DELETE_LINES");
	tracker.gFinder.gLineChecker.minDistance = panel.getValueF("GLINT_MIN_DISTANCE");
	tracker.gFinder.gLineChecker.maxDistance = panel.getValueF("GLINT_MAX_DISTANCE");
	tracker.gFinder.bUseRecordRef = panel.getValueB("B_USE_RECORDED_GRINTREF");
	tracker.gFinder.minDistancePct = panel.getValueF("GLINT_MIN_DIS_PCT");
	tracker.gFinder.maxDistancePct = panel.getValueF("GLINT_MAX_DIS_PCT");
	
	tracker.bUseGlintinBrightEye = panel.getValueB("B_BRIGHT_GLINT");

	
	if (IM.mode != INPUT_VIDEO){
		panel.setWhichPanel("live video settings");
		if (panel.getValueB("VIDEO_SETTINGS") == true){
			
#ifdef TARGET_OSX
			// since macs fuck up bad fullscreen with video settings
			ofSetFullscreen(false);
#endif
			if(IM.grabberType == INPUT_OFVIDEOGRABBER) {
				qtVideoGrabber& cam = *((qtVideoGrabber*) IM.vidGrabber);
				cam.openVideoSettings();
			}
			panel.setValueB("VIDEO_SETTINGS", false);
		} 		
		
	} else {
		IM.vidPlayer.setSpeed(panel.getValueF("VIDEOSPEED"));
	}
}

//--------------------------------------------------------------



