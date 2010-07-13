/*
 *  controlPanelSettings.cpp
 *  EyeTrackDTTest
 *
 *  Created by itotaka on 5/18/10.
 *  Copyright 2010 YCAM. All rights reserved.
 *
 */


#include "trackingManager.h"
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
	
	//	if (IM.mode == INPUT_VIDEO){
	//		panel.addPanel("video file settings", 1, false);
	//	} else {
	//		panel.addPanel("live video settings", 1, false);
	//	}
	
	
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
	panel.addToggle("flip horizontal ", "B_RIGHT_FLIP_X", false);
	panel.addToggle("flip vertical ", "B_RIGHT_FLIP_Y", false);
	
	panel.addToggle("num of glints(0: 2, 1: 4)", "B_FOUR_GLINTS", true);
	panel.addToggle("Use Homography", "B_USE_HOMOGRAPHY", false);

	
	
	// eye positon
	
	panel.setWhichPanel("eye detection 1");
	
	panel.addSlider("threshold(eye)", "THRESHOLD_EYEPOS", 60, 0, 255, true);
	
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
	
	panel.addSlider("threshold_pupil", "THRESHOLD_PUPIL", 60, 0, 255, true);
	
	panel.addSlider("min blob(pupil)","MIN_BLOB_PUPIL",50*10,0,5000,true);
	panel.addSlider("max blob(pupil)","MAX_BLOB_PUPIL",100*100,0,50500,true);
	
	panel.addSlider("maxblobWidth", "MAX_BLOB_WIDTH_PUPIL", 300, 0, 500, false);
	panel.addSlider("maxblobHeight", "MAX_BLOB_HEIGHT_PUPIL", 300, 0, 500, false);
	
	panel.addToggle("use erode/dilate(pupil)", "B_USE_DILATE_PUPIL", true);
	panel.addSlider("erode num", "N_EROSIONS_PUPIL", 0, 0, 20, true);
	panel.addSlider("dilate num ", "N_DILATIONS_PUPIL", 0, 0, 20, true);
	
	panel.addToggle("use blur", "B_USE_BLUR_PUPIL", true);
	panel.addSlider("blur ", "BLUR_PUPIL", 5, 0, 20, false);
	
	panel.addToggle("use contrast stretch", "B_USE_STRETCH_PUPIL", true);
	
	
	
	// pupil position 2
	panel.setWhichPanel("pupil detection 2");
	//	panel.addToggle("UseAdaptiveThreshold", "B_USE_ADAPTIVE_PUPIL", false);
	//	panel.addSlider("blocksize", "BLOCK_SIZE_PUPIL", 200, 0, 1000, true);
	//	panel.addSlider("offset", "OFFSET_PUPIL", 27, 0, 100, true);
	//	panel.addToggle("gauss", "GAUSS_PUPIL", false);
	
	panel.addSlider("scan X", "SCAN_X_PUPIL", 0, 0, 287, true);
	panel.addSlider("scan Y", "SCAN_Y_PUPIL", 0, 0, 255, true);
	
	panel.addToggle("use gamma ", "B_USE_GAMMA_PUPIL", true);
	panel.addSlider("gamma ", "GAMMA_PUPIL", 0.57f, 0.01, 3.0f, false);
	
	panel.addToggle("use contrast / bri", "B_USE_CONTRAST_PUPIL", false);
	panel.addSlider("contrast ", "CONTRAST_PUPIL", 0.28f, 0.0, 1.0f, false);
	panel.addSlider("brightness ", "BRIGHTNESS_PUPIL", -0.02f, -1.0, 3.0f, false);
	
	
	
	
	
	// glint position
	panel.setWhichPanel("glint detection");
	//	panel.addSlider("threshold(bright/dark) ", "THRESHOLD_BD", 170, 0, 255, true);
	panel.addSlider("threshold(glint)", "THRESHOLD_GLINT", 190, 0, 255, true);
		
	panel.addSlider("w/h limiting ratio", "WH_RATIO", 0.7, 0.1, 1, false);
	
	panel.addSlider("min blob(glint)","MIN_BLOB_GLINT",5,0,2000,true);
	panel.addSlider("max blob(glint)","MAX_BLOB_GLINT",500,0,20000,true);
	
	panel.addToggle("draw line(pupil)", "B_DRAW_LINE_P", true);
	panel.addToggle("draw line(glint)", "B_DRAW_LINE_G", true);
	panel.addToggle("draw contour", "B_DRAW_CONTOUR", false);
	
	panel.addToggle("Use contrast stretch ", "B_CONTRAST_STRETCH", true);
	panel.addSlider("range Min", "RANGE_MIN_GLINT", 0, -500, 500, false);
	panel.addSlider("range Max", "RANGE_MAX_GLINT", 255, -255, 1000, false);
	
	
	
	panel.loadSettings("settings/trackingSettings.xml");
	
	
}



//--------------------------------------------------------------
void trackingManager::updateGui(){
	
	// image adjustment
	
	IM.flipX = panel.getValueB("B_RIGHT_FLIP_X");
	IM.flipY = panel.getValueB("B_RIGHT_FLIP_Y");
	
	bFocusScreenMode = panel.getValueB("B_FOCUS_SCREEN");
	
	
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
	tracker.eFinder.blur				= panel.getValueF("BLUR_EYEPOS");
	
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
	
	tracker.pFinder.bUseContrastStretch = panel.getValueB("B_USE_STRETCH_PUPIL");
	
	tracker.threshold_p				= panel.getValueF("THRESHOLD_PUPIL");
	tracker.minBlobSize_p			= panel.getValueF("MIN_BLOB_PUPIL");
	tracker.maxBlobSize_p			= panel.getValueF("MAX_BLOB_PUPIL");
	
	tracker.pFinder.maxWidth		= panel.getValueF("MAX_BLOB_WIDTH_PUPIL");
	tracker.pFinder.maxHeight		= panel.getValueF("MAX_BLOB_HEIGHT_PUPIL");
	
	
	// Pupil Position 2
	//	
	//	tracker.pFinder.bUseAdaptiveThreshold = panel.getValueB("B_USE_ADAPTIVE_PUPIL");
	//	tracker.pFinder.blocksize = panel.getValueF("BLOCK_SIZE_PUPIL");
	//	tracker.pFinder.offset = panel.getValueF("OFFSET_PUPIL");
	//	tracker.pFinder.bGauss = panel.getValueB("GAUSS_PUPIL");
	
	scanX = panel.getValueF("SCAN_X_PUPIL");
	scanY = panel.getValueF("SCAN_Y_PUPIL");
	
	
	// Glint Position
	
	tracker.gFinder.bFourGlints		= panel.getValueB("B_FOUR_GLINTS");
	tracker.bUseHomography	= panel.getValueB("B_USE_HOMOGRAPHY");
	
	tracker.gFinder.limitratio = panel.getValueF("WH_RATIO");
	
	tracker.threshold_g				= panel.getValueF("THRESHOLD_GLINT");
	tracker.minBlobSize_g			= panel.getValueF("MIN_BLOB_GLINT");
	tracker.maxBlobSize_g			= panel.getValueF("MAX_BLOB_GLINT");
	
	tracker.pFinder.bUseContrastStretch = panel.getValueB("B_CONTRAST_STRETCH");
	tracker.pFinder.convMin = panel.getValueF("RANGE_MIN_GLINT");
	tracker.pFinder.convMax = panel.getValueF("RANGE_MAX_GLINT");
	
	if (IM.mode != INPUT_VIDEO){
		panel.setWhichPanel("live video settings");
		if (panel.getValueB("VIDEO_SETTINGS") == true){
			
#ifdef TARGET_OSX
			// since macs fuck up bad fullscreen with video settings
			ofSetFullscreen(false);
#endif
			if(IM.grabberType == INPUT_OFVIDEOGRABBER) {
				ofVideoGrabber& cam = *((ofVideoGrabber*) IM.vidGrabber);
				cam.videoSettings();
			}
			panel.setValueB("VIDEO_SETTINGS", false);
			
		} 		
		
	} else {
		
		IM.vidPlayer.setSpeed(panel.getValueF("VIDEOSPEED"));
		
	}
	
}

//--------------------------------------------------------------
