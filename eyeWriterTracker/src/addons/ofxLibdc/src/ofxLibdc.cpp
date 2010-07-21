#include "ofxLibdc.h"

dc1394_t* ofxLibdc::libdcContext = NULL;
int ofxLibdc::libdcCameras = 0;

ofxLibdc::ofxLibdc() :
		camera(NULL),
		imageType(OF_IMAGE_COLOR),
		useFormat7(false),
		use1394b(false),
		width(640),
		height(480),
		left(0),
		top(0),
		format7Mode(0),
		capturePolicy(DC1394_CAPTURE_POLICY_POLL) {
	startLibdcContext();
}

ofxLibdc::~ofxLibdc() {
	if(camera != NULL) {
		dc1394_capture_stop(camera);
		setTransmit(false);
		dc1394_camera_free(camera);
	}
	stopLibdcContext();
}

void ofxLibdc::setBlocking(bool blocking) {
	capturePolicy = blocking ? DC1394_CAPTURE_POLICY_WAIT : DC1394_CAPTURE_POLICY_POLL;
}

void ofxLibdc::startLibdcContext() {
	if(libdcCameras == 0) {
		ofLog(OF_LOG_VERBOSE, "Creating libdc1394 context with dc1394_new().");
		libdcContext = dc1394_new();
	}
	libdcCameras++;
}

void ofxLibdc::stopLibdcContext() {
	libdcCameras--;
	if(libdcCameras == 0) {
		ofLog(OF_LOG_VERBOSE, "No more cameras, destroying libdc1394 context.");
		dc1394_free(libdcContext);
	}
}

int ofxLibdc::getOfImageType(dc1394color_coding_t libdcType) {
	switch(libdcType) {
		case DC1394_COLOR_CODING_MONO8: return OF_IMAGE_GRAYSCALE;
		case DC1394_COLOR_CODING_RGB8: return OF_IMAGE_COLOR;
		default: return OF_IMAGE_COLOR;
	}
}

dc1394color_coding_t ofxLibdc::getLibdcType(int ofImageType) {
	switch(ofImageType) {
		case OF_IMAGE_GRAYSCALE: return DC1394_COLOR_CODING_MONO8;
		case OF_IMAGE_COLOR: return DC1394_COLOR_CODING_RGB8;
		default: return DC1394_COLOR_CODING_RGB8;
	}
}

int ofxLibdc::getCameraCount() {
	dc1394camera_list_t* list;
	dc1394_camera_enumerate (libdcContext, &list);
	int cameraCount = list->num;
	dc1394_camera_free_list (list);
	return cameraCount;
}

void ofxLibdc::setFormat7(bool useFormat7, int mode) {
	bool changed = useFormat7 != this->useFormat7 || mode != this->format7Mode;
	this->useFormat7 = useFormat7;
	this->format7Mode = mode;
	if(camera && changed)
		applySettings();
}

void ofxLibdc::set1394b(bool use1394b) {
	bool changed = use1394b != this->use1394b; 
	this->use1394b = use1394b;
	if(camera && changed)
		applySettings();
}

void ofxLibdc::setSize(unsigned int width, unsigned int height) {
	bool changed = width != this->width || height != this->height; 
	this->width = width;
	this->height = height;
	if(camera && changed)
		applySettings();
}

void ofxLibdc::setPosition(unsigned int left, unsigned int top) {
	bool changed = left != this->left || top != this->top; 
	this->left = left;
	this->top = top;
	if(camera && changed) {
		quantizePosition();
		dc1394_format7_set_image_position(camera, videoMode, this->left, this->top);
	}
}

void ofxLibdc::setup(int cameraNumber) {
	initCamera(cameraNumber);
	applySettings();
}

void ofxLibdc::initCamera(int cameraNumber) {
	// create camera struct
	dc1394camera_list_t* list;
	dc1394_camera_enumerate(libdcContext, &list);
	if(list->num == 0) {
		ofLog(OF_LOG_ERROR, "No cameras found.");
		return;
	}
	camera = dc1394_camera_new(libdcContext, list->ids[cameraNumber].guid);
	if (!camera) {
		stringstream error;
		error << "Failed to initialize camera " << cameraNumber << " with guid " << list->ids[cameraNumber].guid;
		ofLog(OF_LOG_ERROR, error.str());
		return;
	} else {
		stringstream msg;
		msg << "Using camera with GUID " << camera->guid;
		ofLog(OF_LOG_VERBOSE, msg.str());
	}
	dc1394_camera_free_list(list);
	
	#ifdef TARGET_OSX
	dc1394_iso_release_bandwidth(camera, INT_MAX);
	for (int channel = 0; channel < 64; channel++)
		dc1394_iso_release_channel(camera, channel);
	#endif
	
	#ifdef TARGET_LINUX
	dc1394_reset_bus(camera);
	#endif
}

void ofxLibdc::applySettings() {
	if(camera)
		dc1394_capture_stop(camera);
		
	if(use1394b) {
		// assumes you want to run your 1394b camera at 800 Mbps
		dc1394_video_set_operation_mode(camera, DC1394_OPERATION_MODE_1394B);
		dc1394_video_set_iso_speed(camera, DC1394_ISO_SPEED_800);
	} else {
		dc1394_video_set_operation_mode(camera, DC1394_OPERATION_MODE_LEGACY);
		dc1394_video_set_iso_speed(camera, DC1394_ISO_SPEED_400);
	}
		
	dc1394framerate_t framerate;
	if(useFormat7) {
		videoMode = (dc1394video_mode_t) ((int) DC1394_VIDEO_MODE_FORMAT7_0 + format7Mode);
	} else {
		dc1394video_modes_t video_modes;
		dc1394_video_get_supported_modes(camera, &video_modes);
		dc1394color_coding_t coding;
		dc1394color_coding_t targetCoding = getLibdcType(imageType);
		for (int i = 0; i < video_modes.num; i++) {
			if (!dc1394_is_video_mode_scalable(video_modes.modes[i])) {
				dc1394video_mode_t curMode = video_modes.modes[i];
				dc1394_get_color_coding_from_video_mode(camera, curMode, &coding);
				unsigned int curWidth, curHeight;
				dc1394_get_image_size_from_video_mode(camera, curMode, &curWidth, &curHeight);
				if (coding == targetCoding && width == curWidth && height == curHeight) {
					videoMode = curMode;
					break;
				}
			}
			if(i == video_modes.num - 1) {
				ofLog(OF_LOG_ERROR, "Camera does not support target mode.");
				return;
			}
		}
		
		// get fastest framerate
		// todo: make this settable
		dc1394framerates_t framerates;
		dc1394_video_get_supported_framerates(camera, videoMode, &framerates);
		framerate = framerates.framerates[framerates.num - 1];
	}
	
	if(useFormat7) {
		quantizePosition();
		quantizeSize();
		dc1394_format7_set_roi(camera, videoMode, getLibdcType(imageType), DC1394_USE_MAX_AVAIL, left, top, width, height);
		unsigned int curWidth, curHeight;
		dc1394_format7_get_image_size(camera, videoMode, &curWidth, &curHeight);
	} else {
		dc1394_video_set_framerate(camera, framerate);
	}
	
	// contrary to the libdc1394 format7 demo, this should go after the roi setting
	dc1394_video_set_mode(camera, videoMode);
		
	dc1394_capture_setup(camera, OFXLIBDC_BUFFER_SIZE, DC1394_CAPTURE_FLAGS_DEFAULT);
}

void ofxLibdc::quantizePosition() {
	unsigned int hunit, vunit;
	dc1394_format7_get_unit_position(camera, videoMode, &hunit, &vunit);
	left = (left / hunit) * hunit;
	top = (top / vunit) * vunit;
}

void ofxLibdc::quantizeSize() {
	unsigned int hunit, vunit;
	dc1394_format7_get_unit_size(camera, videoMode, &hunit, &vunit);
	width = (width / hunit) * hunit;
	height = (height / vunit) * vunit;
}

void ofxLibdc::printFeatures() const {
	dc1394featureset_t features;
	dc1394_feature_get_all(camera, &features);
	dc1394_feature_print_all(&features, stdout);
}

void ofxLibdc::setBrightness(unsigned int brightness) {
	setFeature(DC1394_FEATURE_BRIGHTNESS, brightness);
}

void ofxLibdc::setGamma(unsigned int gamma) {
	setFeature(DC1394_FEATURE_GAMMA, gamma);
}

void ofxLibdc::setGain(unsigned int gain) {
	setFeature(DC1394_FEATURE_GAIN, gain);
}

void ofxLibdc::setExposure(unsigned int exposure) {
	setFeature(DC1394_FEATURE_EXPOSURE, exposure);
}

void ofxLibdc::setShutter(unsigned int shutter) {
	setFeature(DC1394_FEATURE_SHUTTER, shutter);
}

void ofxLibdc::setBrightnessNorm(float brightness) {
	setFeatureNorm(DC1394_FEATURE_BRIGHTNESS, brightness);
}

void ofxLibdc::setGammaNorm(float gamma) {
	setFeatureNorm(DC1394_FEATURE_GAMMA, gamma);
}

void ofxLibdc::setGainNorm(float gain) {
	setFeatureNorm(DC1394_FEATURE_GAIN, gain);
}

void ofxLibdc::setExposureNorm(float exposure) {
	setFeatureNorm(DC1394_FEATURE_EXPOSURE, exposure);
}

void ofxLibdc::setShutterNorm(float shutter) {
	setFeatureNorm(DC1394_FEATURE_SHUTTER, shutter);
}

void ofxLibdc::setFeature(dc1394feature_t feature, unsigned int value) {
	dc1394_feature_set_power(camera, feature, DC1394_ON);
	dc1394_feature_set_mode(camera, feature, DC1394_FEATURE_MODE_MANUAL);
	dc1394_feature_set_value(camera, feature, value);
}

void ofxLibdc::setFeatureNorm(dc1394feature_t feature, float value) {
	unsigned int min, max;
	getFeatureRange(feature, &min, &max);
	dc1394_feature_set_power(camera, feature, DC1394_ON);
	dc1394_feature_set_mode(camera, feature, DC1394_FEATURE_MODE_MANUAL);
	dc1394_feature_set_value(camera, feature, value * (max - min) + min);
}

void ofxLibdc::getBrightnessRange(unsigned int* min, unsigned int* max) const {
	getFeatureRange(DC1394_FEATURE_BRIGHTNESS, min, max);
}

void ofxLibdc::getGammaRange(unsigned int* min, unsigned int* max) const {
	getFeatureRange(DC1394_FEATURE_GAMMA, min, max);
}

void ofxLibdc::getGainRange(unsigned int* min, unsigned int* max) const {
	getFeatureRange(DC1394_FEATURE_GAIN, min, max);
}

void ofxLibdc::getExposureRange(unsigned int* min, unsigned int* max) const {
	getFeatureRange(DC1394_FEATURE_EXPOSURE, min, max);
}

void ofxLibdc::getShutterRange(unsigned int* min, unsigned int* max) const {
	getFeatureRange(DC1394_FEATURE_SHUTTER, min, max);
}

void ofxLibdc::getFeatureRange(dc1394feature_t feature, unsigned int* min, unsigned int* max) const {
	dc1394_feature_get_boundaries(camera, feature, min, max);	
}

unsigned int ofxLibdc::getBrightness() {
	return getFeature(DC1394_FEATURE_BRIGHTNESS);
}

unsigned int ofxLibdc::getGamma() {
	return getFeature(DC1394_FEATURE_GAMMA);
}

unsigned int ofxLibdc::getGain() {
	return getFeature(DC1394_FEATURE_GAIN);
}

unsigned int ofxLibdc::getExposure() {
	return getFeature(DC1394_FEATURE_EXPOSURE);
}

unsigned int ofxLibdc::getShutter() {
	return getFeature(DC1394_FEATURE_SHUTTER);
}

unsigned int ofxLibdc::getFeature(dc1394feature_t feature) {
	unsigned int value;
	dc1394_feature_get_value(camera, feature, &value);
	return value;
}

float ofxLibdc::getBrightnessNorm() {
	return getFeatureNorm(DC1394_FEATURE_BRIGHTNESS);
}

float ofxLibdc::getGammaNorm() {
	return getFeatureNorm(DC1394_FEATURE_GAMMA);
}

float ofxLibdc::getGainNorm() {
	return getFeatureNorm(DC1394_FEATURE_GAIN);
}

float ofxLibdc::getExposureNorm() {
	return getFeatureNorm(DC1394_FEATURE_EXPOSURE);
}

float ofxLibdc::getShutterNorm() {
	return getFeatureNorm(DC1394_FEATURE_SHUTTER);
}

float ofxLibdc::getFeatureNorm(dc1394feature_t feature) {
	unsigned int value = getFeature(feature);
	unsigned int min, max;
	getFeatureRange(feature, &min, &max);
	return ((float) value - min) / (max - min);
}

float ofxLibdc::getShutterAbs() const {
	return getFeatureAbs(DC1394_FEATURE_SHUTTER);
}

float ofxLibdc::getFeatureAbs(dc1394feature_t feature) const {
	float value;
	dc1394_feature_get_absolute_value(camera, feature, &value);
	return value;
}

unsigned int ofxLibdc::getWidth() const {
	return width;
}

unsigned int ofxLibdc::getHeight() const {
	return height;
}

int ofxLibdc::getImageType() const {
	return imageType;
}

void ofxLibdc::setImageType(int imageType) {
	this->imageType = imageType;
}

void ofxLibdc::setTransmit(bool transmit) {
	dc1394switch_t cur, target;
	dc1394_video_get_transmission(camera, &cur);
	target = transmit ? DC1394_ON : DC1394_OFF;
	if(cur != target)
		dc1394_video_set_transmission(camera, target);
}

void ofxLibdc::grabStill(ofImage& img) {
	setTransmit(false);
	flushBuffer();
	dc1394_video_set_one_shot(camera, DC1394_ON);
	// if possible, the following should be replaced with a call to grabFrame
	dc1394video_frame_t *frame;
	dc1394_capture_dequeue(camera, capturePolicy, &frame);
	img.allocate(width, height, imageType);
	if(imageType == OF_IMAGE_GRAYSCALE) {
		memcpy(img.getPixels(), frame->image, width * height);
	} else if(imageType == OF_IMAGE_COLOR) {
	}
	dc1394_capture_enqueue(camera, frame);
}

bool ofxLibdc::grabVideo(ofImage& img, bool dropFrames) {
	setTransmit(true);
	img.allocate(width, height, imageType);
	if(dropFrames) {
		bool remaining;
		int i = 0;
		do {
			remaining = grabFrame(img);
			if(!remaining && i == 0)
				return false;
			i++;
		} while (remaining);
		return true;
	} else {
		return grabFrame(img);
	}
}

bool ofxLibdc::grabFrame(ofImage& img) {
	dc1394video_frame_t *frame;
	dc1394_capture_dequeue(camera, capturePolicy, &frame);
	if(frame != NULL) {
		if(imageType == OF_IMAGE_GRAYSCALE) {
			memcpy(img.getPixels(), frame->image, width * height);
		} else if(imageType == OF_IMAGE_COLOR) {
		}
		dc1394_capture_enqueue(camera, frame);
		return true;
	} else {
		return false;
	}
}

void ofxLibdc::flushBuffer() {
	dc1394video_frame_t *frame;
	do {
		dc1394_capture_dequeue(camera, DC1394_CAPTURE_POLICY_POLL, &frame);
		if(frame != NULL)
			dc1394_capture_enqueue(camera, frame);
	} while (frame != NULL);
}

dc1394camera_t* ofxLibdc::getLibdcCamera() {
	return camera;
}
