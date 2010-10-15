/*
 *  groupStyle.h
 *  openFrameworks
 *
 *  Created by Keith on 8/21/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */
#pragma once
#include <string>
#include <map>

using namespace std;

class groupStyle{
	public:
	
		groupStyle(){ init(0); };
		groupStyle(int l){ init(l); }
		virtual ~groupStyle(){};
	
		int layer;
	
//		int strokeWeight;		// deprecated
//		int brushId;			// deprecated
//		int dropShadow[2];		// deprecated
		bool dotShadow;
		bool shadowHash;
		int outline;
		bool doFill;
		bool doHoles;
	
//		int strokeColor;		// deprecated
//		int fillColor;			// deprecated
//		int shadowColor;		// deprecated
//		 int outlineColor;		// deprecated
	
		map<string,int> colors;
		map<string,int> atts;
	
		void init(int l){
			
			colors["fillColor"] = 0xffffff;
			colors["shadowColor"] = 0xffffff;
			colors["strokeColor"] = 0x000000;
			colors["outlineColor"] = 0xaaaaaa;
			
			atts["strokeWeight"] = 2;
			atts["dropShadowX"] = 0;
			atts["dropShadowY"] = 0;
			atts["outline"] = 0;
			atts["brushId"] = 0;
			
			doFill            = false;
			doHoles			  = false;
			outline           = 0;
			layer             = l;
//			strokeWeight      = 2;						//deprecated
//			brushId           = 0;						//deprecated
			dotShadow         = false;
			shadowHash        = false;
//			dropShadow[0] = 0; dropShadow[1] = 0;		//deprecated
			
			/* deprecated 
			fillColor		  = 0xffffff;
			shadowColor		  = 0xffffff; 
			strokeColor		  = 0x000000;
			outlineColor      = 0xaaaaaa;
			 */
		}
};