ofxLibdc is an Open Frameworks wrapper for libdc1394 that makes it easy to grab images from libdc1394-compatible cameras.

http://openframeworks.cc/
http://damien.douxchamps.net/ieee1394/libdc1394/

It avoids unnecessary threading by using libdc1394's non-blocking image grabbing, which means you can call grabVideo without worrying about it slowing down your application.

Instead of copying the image to an internal buffer before giving it to you, ofxLibdc will fill the image you pass to it. This avoids unnecessary copying.

Grabbing with ofxLibdc looks like:

ofLibdc camera;
ofImage currentFrame;
if(camera.grabVideo(currentFrame)) {
	// do something with currentFrame
}

Because there is no separate capture thread, there is no overhead from copying images you don't need.

ofxLibdc can dynamically changed a number of parameters. setPosition() can be used to change the ROI position without restarting the camera. Other changes can be made, but will cause slight delays. Format 7 can be switched on and off, or between modes, 1394b can be switched on and off, and the ROI can be resized.