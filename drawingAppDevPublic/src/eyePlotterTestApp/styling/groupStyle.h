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
		map<string,bool> toggs;
	
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
			
			toggs["doFill"] = false;
			toggs["doHoles"] = false;
			toggs["dotShadow"] = false;
			toggs["shadowHash"] = false;
			
			layer             = l;
			
			/* deprecated
			doFill            = false;
			doHoles			  = false;
			outline           = 0;						
			strokeWeight      = 2;						
			brushId           = 0;						
			dotShadow         = false;
			shadowHash        = false;
			dropShadow[0] = 0; dropShadow[1] = 0;	
			*/
			
			/* deprecated 
			fillColor		  = 0xffffff;
			shadowColor		  = 0xffffff; 
			strokeColor		  = 0x000000;
			outlineColor      = 0xaaaaaa;
			 */
		}
};