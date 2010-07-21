/*
 ofxLibdcPtGrey extends ofxLibdcGrabber, meaning you can use either the
 common ofBaseVideo style grabFrame()/isFrameNew() or the ofxLibdc style
 grabVideo(ofImage).
 
 ofxLibdcPtGrey wraps some functionality specific to Point Grey cameras.
 For example, you can embed and retrieve frame-specifc information in the
 returned pixels using setEmbeddedInfo() and getEmbeddedInfo().
 
 setMaxFramerate() queries that camera for the maximum framerate it can
 run at, and sets the framerate register to that value.
 
 setupAlternatingStrobe() is a useful strobe pattern that will output a
 pulse on GPIO 0 and 1 on alternating frames. The strobe counter for a
 given frame can be retrieved using getEmbeddedStrobCounter().
 */

#pragma once

#include "ofxLibdcGrabber.h"

#define PTGREY_FRAME_RATE_INQ 0x53c
#define PTGREY_FRAME_RATE 0x83c
#define PTGREY_FRAME_INFO 0x12f8

#define PTGREY_PIO_DIRECTION 0x11f8
#define PTGREY_STROBE_0_CNT 0x1500
#define PTGREY_STROBE_1_CNT 0x1504
#define PTGREY_GPIO_STRPAT_CTRL 0x110c
#define PTGREY_GPIO_CTRL_PIN_0 0x1110
#define PTGREY_GPIO_CTRL_PIN_1 0x1120
#define PTGREY_GPIO_STRPAT_MASK_PIN_0 0x1118
#define PTGREY_GPIO_STRPAT_MASK_PIN_1 0x1128

enum ptGreyEmbed {
	PTGREY_EMBED_TIMESTAMP = 0,
	PTGREY_EMBED_GAIN,
	PTGREY_EMBED_SHUTTER,
	PTGREY_EMBED_BRIGHTNESS,
	PTGREY_EMBED_EXPOSURE,
	PTGREY_EMBED_WHITE_BALANCE,
	PTGREY_EMBED_FRAME_COUNTER,
	PTGREY_EMBED_STROBE_PATTERN,
	PTGREY_EMBED_GPIO,
	PTGREY_EMBED_ROI
};

class ofxLibdcPtGrey : public ofxLibdcGrabber {
public:
	void setupAlternatingStrobe();
	
	void clearEmbeddedInfo();
	void setEmbeddedInfo(int embeddedInfo, bool enable = true);
	
	unsigned int getEmbeddedInfo(unsigned char* pixels, int embeddedInfo) const;
	void getEmbeddedPosition(unsigned char* pixels, unsigned short* left, unsigned short* top) const;
	unsigned int getEmbeddedStrobeCounter(unsigned char* pixels) const;
	
	void setMaxFramerate();
protected:
	unsigned int getEmbeddedInfoOffset(int embeddedInfo) const;
};