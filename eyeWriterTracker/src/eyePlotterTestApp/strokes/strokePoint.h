/*
 *  strokePoint.h
 *  openFrameworks
 *
 *  Created by theo on 17/08/2009.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */
 #pragma once
 #include "ofMain.h"

class strokePoint : public ofPoint{
	public:	
		strokePoint(float _x, float _y, float _t, float _confidence){
			x			= _x;
			y			= _y;
			t			= _t;
			confidence	= _confidence;
		}
		
		float confidence;
		float t;
};
