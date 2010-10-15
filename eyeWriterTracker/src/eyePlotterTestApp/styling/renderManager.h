/*
 *	renderManager.h
 *  openFrameworks
 *
 *  Created by Keith on 8/21/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#pragma once

#include "strokeGroup.h"
#include "groupStyle.h"

struct fPrint{
	vector<float> vals;
	float operator[](int i){
		while(i+1>vals.size())vals.push_back(ofRandomuf());
		return vals[i];
	}
};

class renderManager{
	public:
	fPrint rnd;
	ofImage dot;
	int numBrushes;
	
	renderManager(){ 
		numBrushes = 3;
	}
		
	/*
	struct compare{
		bool operator() (const strokeGroup& a,const strokeGroup& b) { return a.style.layer > b.style.layer;}
	}layerSort;
	*/
	void render(vector<strokeGroup>& groups){
		ofPushStyle();
		
		if( dot.width == 0){
			dot.loadImage("resources/dot2.png");
			dot.setAnchorPercent(50, 50);
		}
		
//		std::sort(groups.begin(),groups.end(),layerSort);
			
		for(int i = groups.size(); --i >= 0; ){
			strokeGroup& curGroup = groups[i];
			groupStyle& curStyle = curGroup.style;
			if (curStyle.outline > 0){

				ofEnableAlphaBlending();
				dot.getTextureReference().bind();
				ofSetColor(curStyle.colors["outlineColor"]);
				int shiftX = curStyle.atts["dropShadowX"];
				int shiftY = curStyle.atts["dropShadowY"];
				
				for (int s = 0; s < curGroup.strokes.size(); s++){
					stroke& curStroke = curGroup.strokes[s];
					for (int p = 0; p < curStroke.pts.size() - 1; p++){
						ofxVec2f dir = curStroke.pts[p+1] - curStroke.pts[p];
						float l = dir.lengthSquared();
						float step = 60.0/l + .001*curStyle.outline;
						
						for (float d = 0; d <= 1; d += step ){				
							float offset = curStyle.outline + curStyle.atts["strokeWeight"];
							ofFill();
							//ofCircle(groups[i].strokes[s].pts[p].x + (d*dir).x + shiftX, groups[i].strokes[s].pts[p].y + (d*dir).y + shiftY, offset);
							//ofCircle(groups[i].strokes[s].pts[p].x + (d*dir).x         , groups[i].strokes[s].pts[p].y + (d*dir).y, offset);
							
							glBegin(GL_QUADS);
							glTexCoord2i(0,0);
							glVertex2f(curStroke.pts[p].x  + (d*dir).x - offset, curStroke.pts[p].y + (d*dir).y - offset);
							glTexCoord2i(80,0);
							glVertex2f(curStroke.pts[p].x  + (d*dir).x + offset, curStroke.pts[p].y +(d*dir).y - offset);
							glTexCoord2i(80,80);
							glVertex2f(curStroke.pts[p].x  + (d*dir).x + offset, curStroke.pts[p].y + (d*dir).y + offset);
							glTexCoord2i(0,80);
							glVertex2f(curStroke.pts[p].x  + (d*dir).x - offset, curStroke.pts[p].y + (d*dir).y + offset);
							glEnd();
							
							glBegin(GL_QUADS);
							glTexCoord2i(0,0);
							glVertex2f(curStroke.pts[p].x + shiftX  + (d*dir).x - offset, curStroke.pts[p].y + (d*dir).y + shiftY - offset);
							glTexCoord2i(80,0);
							glVertex2f(curStroke.pts[p].x + shiftX  + (d*dir).x + offset, curStroke.pts[p].y +(d*dir).y + shiftY - offset);
							glTexCoord2i(80,80);
							glVertex2f(curStroke.pts[p].x + shiftX  + (d*dir).x + offset, curStroke.pts[p].y + (d*dir).y + shiftY + offset);
							glTexCoord2i(0,80);
							glVertex2f(curStroke.pts[p].x + shiftX + (d*dir).x - offset, curStroke.pts[p].y + (d*dir).y + shiftY + offset);
							glEnd();
							
						}
					}
				}
				dot.getTextureReference().unbind();
				ofDisableAlphaBlending();

			}
		}


	////////////////////////////////////////////////////////////////
		for(int i = 0; i < groups.size(); ++i ){
			strokeGroup& curGroup = groups[i];
			groupStyle& curStyle = curGroup.style;
			int shiftX = curStyle.atts["dropShadowX"];
			int shiftY = curStyle.atts["dropShadowY"];
			
			// ----------------   BOTTOM LAYER
			
			for (int s = 0; s < curGroup.strokes.size(); s++){
				stroke& curStroke = curGroup.strokes[s];

				if (curStyle.atts["strokeWeight"]!=0){
					/// BACK STROKE ///////////////////////////////////////////////
					ofSetColor(curStyle.colors["strokeColor"]);
					ofSetLineWidth(curStyle.atts["strokeWeight"]);
					ofNoFill();
					ofBeginShape();
					for(int p = 0; p < curStroke.pts.size(); p++){
						ofVertex(curStroke.pts[p].x + shiftX, curStroke.pts[p].y + shiftY);
					}
					ofEndShape();
				}
					////////////////////////////////////////////////////////////////			

				
				if (shiftX != 0 || shiftY!=0){
				//// DROP SHADOW BASIC /////////////////////////////////////////
				ofSetColor(curStyle.colors["shadowColor"]);
				for(int p = 0; p < curStroke.pts.size()-1; p++){
					//ofLine(curGroup.strokes[i].pts[k].x, curGroup.strokes[i].pts[k].y,curGroup.strokes[i].pts[k].x-50, curGroup.strokes[i].pts[k].y);
					ofxVec2f dir = curStroke.pts[p+1] - curStroke.pts[p];
					float dot = ofxVec2f(-1,0).dot(dir);
					if (i == 1) dot*=-1;
					(curStyle.dotShadow) ? ((dot < 0) ? ofSetColor(((curStyle.colors["shadowColor"] >> 16) & 0xff)*.4,
																		  ((curStyle.colors["shadowColor"] >> 8) & 0xff)*.4,
																		  ((curStyle.colors["shadowColor"] >> 0) & 0xff)*.4) : ofSetColor(curStyle.colors["shadowColor"])) : ofSetColor(curStyle.colors["shadowColor"]);
					ofFill();
					ofBeginShape();
					ofVertex(curStroke.pts[p].x, curStroke.pts[p].y);
					ofVertex(curStroke.pts[p].x+shiftX, curStroke.pts[p].y+shiftY);
					ofVertex(curStroke.pts[p+1].x+shiftX, curStroke.pts[p+1].y+shiftY);
					ofVertex(curStroke.pts[p+1].x, curStroke.pts[p+1].y);
					ofEndShape();
					}
					
					////////////////////////////////////////////////////////////////
				if (curStyle.shadowHash){
					ofEnableAlphaBlending();
					//// DROP SHADOW HIGHLIGHTS ////////////////////////////////////
					for (int p = 0; p < curStroke.pts.size() - 1; p++){
						ofxVec2f dir = curStroke.pts[p+1] - curStroke.pts[p];
						float l = dir.lengthSquared();
						float step = .1;
						
						for (float d = 0; d < 1; d += step ){
							float test;
							test = rnd[int(p*d*1000.0)];
							float wid = rnd[int(p+d*100.0)]*2;
							if(p > curStroke.pts.size()-4) wid *=.5;
							
							if (d <.3 || d>.7){
								if (test < .4){
									(test > .2) ?  glColor4f(0,0,0,.6*rnd[int(100*d+ p)]) : glColor4f(1,1,1,.6*rnd[int(100*d+p)]);
									
									glBegin(GL_QUADS);
									glVertex2f(curStroke.pts[p].x + (d*dir).x , curStroke.pts[p].y + (d*dir).y);
									glVertex2f(curStroke.pts[p].x + (d*dir).x +shiftX, curStroke.pts[p].y + (d*dir).y + shiftY);
									glVertex2f(curStroke.pts[p].x + ((d+step*wid)*dir).x +shiftX, curStroke.pts[p].y + ((d+step*wid)*dir).y + shiftY);
									glVertex2f(curStroke.pts[p].x + ((d+step*wid)*dir).x , curStroke.pts[p].y + ((d+step*wid)*dir).y);
									glEnd();
								}
							}
						}
					}
					ofDisableAlphaBlending();
				}
				

					if (curStyle.atts["strokeWeight"]!=0){
					/// CORNER LINES //////////////////////////////////////////////
					ofSetColor(curStyle.colors["strokeColor"]);
					ofSetLineWidth(curStyle.atts["strokeWeight"]);
					for(int p = 0; p < curStroke.pts.size(); p++){
						ofLine(curStroke.pts[p].x, curStroke.pts[p].y,curStroke.pts[p].x + shiftX, curStroke.pts[p].y + shiftY);
					}
					}
					////////////////////////////////////////////////////////////////
					
				if (curStyle.atts["strokeWeight"] > 2){
					/// DROP SHADOW CORNER DOTS ///////////////////////////////////
					ofSetColor(curStyle.colors["strokeColor"]);
					ofFill();
					for(int p = 0; p < curStroke.pts.size(); p++){
						ofCircle(curStroke.pts[p].x+shiftX, curStroke.pts[p].y+shiftY,.5*curStyle.atts["strokeWeight"]);
					}
					////////////////////////////////////////////////////////////////
				}
				
			}
			
			}
			
			// ----------------   END BOTTOM LAYER
			
			// ----------------   MIDDLE LAYER
			
			if( curStyle.doFill && curGroup.strokes.size() ){
				
				//printf("DO FILL FOOL!\n");
				
				bool bHoles = curStyle.doHoles;
				if( bHoles ){
			
					ofPushStyle();
						ofSetColor(curStyle.colors["fillColor"]);
						ofFill();
						ofBeginShape();
						for (int s = 0; s < curGroup.strokes.size(); s++){
							/// MAIN LETTER FILL //////////////////////////////////////////
							for(int p = 0; p < curGroup.strokes[s].pts.size(); p++){
								ofVertex(curGroup.strokes[s].pts[p].x, curGroup.strokes[s].pts[p].y);
							}
							ofNextContour(false);
						}			
						ofEndShape();	
					ofPopStyle();
				
				}else{
					ofPushStyle();
						ofFill();
						ofSetColor(curStyle.colors["fillColor"]);														
						for (int s = 0; s < curGroup.strokes.size(); s++){
							ofBeginShape();
								/// MAIN LETTER FILL //////////////////////////////////////////
								for(int p = 0; p < curGroup.strokes[s].pts.size(); p++){
									ofVertex(curGroup.strokes[s].pts[p].x, curGroup.strokes[s].pts[p].y);
								}
							ofEndShape();	
						}				
					ofPopStyle();
				}
			}else{
				//printf("NO FILL FOOL - %i !\n",  groups[i].style.doFill);
			}

			// ----------------   END MIDDLE LAYER

			
			// ----------------   TOP LAYER
			
			for (int s = 0; s < curGroup.strokes.size(); s++){
				stroke& curStroke = curGroup.strokes[s];

				if (curStyle.atts["strokeWeight"]!=0){
				/// MAIN LETTER STROKE ///////////////////////////////////////
				ofSetColor(curStyle.colors["strokeColor"]);
				ofSetLineWidth(curStyle.atts["strokeWeight"]);
					
					switch(curStyle.atts["brushId"]){	
						case(0):
						/// MAIN LETTER STROKE ///////////////////////////////////////
				ofNoFill();
				ofBeginShape();
				for(int p = 0; p < curStroke.pts.size(); p++){
					ofVertex(curStroke.pts[p].x, curStroke.pts[p].y);
				}
				ofEndShape();
		
				
				//// MAIN STROKE CORNER DOTS ///////////////////////////////////
				if (curStyle.atts["strokeWeight"] > 4){
					ofFill();
					for(int p = 0; p < curStroke.pts.size(); p++){
						ofCircle(curStroke.pts[p].x, curStroke.pts[p].y,.5*curStyle.atts["strokeWeight"]);
					}
				}
				////////////////////////////////////////////////////////////////	
						break;
						case(1):
							/// MAIN LETTER STROKE ///////////////////////////////////////
							ofPushStyle();
							ofEnableAlphaBlending();
							dot.getTextureReference().bind();
							for (int s = 0; s < groups[i].strokes.size(); s++){
								for (int p = 0; p < curStroke.pts.size() - 1; p++){
									ofxVec2f dir = curStroke.pts[p+1] - curStroke.pts[p];
									float l = dir.lengthSquared();
									float step = 60.0/l;
									
									for (float d = 0; d <= 1; d += step ){				
										float offset = curStyle.atts["strokeWeight"];
										ofFill();
										//ofCircle(curStroke.pts[p].x + (d*dir).x + shiftX, curStroke.pts[p].y + (d*dir).y + shiftY, offset);
										//ofCircle(curStroke.pts[p].x + (d*dir).x         , curStroke.pts[p].y + (d*dir).y, offset);
										
										glBegin(GL_QUADS);
										glTexCoord2i(0,0);
										glVertex2f(curStroke.pts[p].x  + (d*dir).x - offset, curStroke.pts[p].y + (d*dir).y - offset);
										glTexCoord2i(80,0);
										glVertex2f(curStroke.pts[p].x  + (d*dir).x + offset, curStroke.pts[p].y +(d*dir).y - offset);
										glTexCoord2i(80,80);
										glVertex2f(curStroke.pts[p].x  + (d*dir).x + offset, curStroke.pts[p].y + (d*dir).y + offset);
										glTexCoord2i(0,80);
										glVertex2f(curStroke.pts[p].x  + (d*dir).x - offset, curStroke.pts[p].y + (d*dir).y + offset);
										glEnd();
										
									}
								}
							}
							dot.getTextureReference().unbind();
							ofDisableAlphaBlending();
							ofPopStyle();
						break;
						case(2):
							/// MAIN LETTER STROKE ///////////////////////////////////////
							ofPushStyle();
							for (int s = 0; s < curGroup.strokes.size(); s++){
								for (int p = 0; p < curStroke.pts.size() - 1; p++){
									float offset = 3+curStyle.atts["strokeWeight"];
									ofFill();
									ofTriangle(curStroke.pts[p].x, curStroke.pts[p].y, curStroke.pts[p+1].x + offset,
											   curStroke.pts[p+1].y + offset, curStroke.pts[p+1].x - offset,
											   curStroke.pts[p+1].y - offset);
								}
							}

							ofPopStyle();
							break;
							
						default: break;
					}
				}
				////////////////////////////////////////////////////////////////
			}
			
			// ----------------   END TOP LAYER
		}
			
		ofPopStyle();
	}



};
