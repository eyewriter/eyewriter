#include "grafIO.h"
#include "strokeUtils.h"

grafIO::grafIO()
{
    
	
	
}

grafIO::~grafIO()
{
    //dtor
}

void grafIO::saveTag(strokeGroup & tag)
{
	// check if file already exists
	string sec		= "_" + ofToString( ofGetSeconds() );
	string min		= "_" + ofToString( ofGetMinutes() );
	string hour		= "_" + ofToString( ofGetHours() );
	string day		= "_" + ofToString( ofGetDay() );
	string month	= "_" + ofToString( ofGetMonth() );
	string year		= ofToString( ofGetYear() );
	
	saveTag(tag,"tempt-"+year+month+day+hour+min+sec+".gml");
}


void grafIO::saveTag(strokeGroup & tag, string filename)
{
	
	constructGML(tag);
	xml.saveFile(filename);
	
	
}

void grafIO::constructGML(strokeGroup & tag )
{
	if( tag.strokes.size() <= 0){
		return;
	}
	
	ofRectangle rect = tag.getBoundingRect();
	if( rect.width <= 0 || rect.height <= 0 ){
		return;
	}
	
	float startTime = tag.getStartTime();
	
	xml.clear();
	
	xml.addTag("GML");
	//xml.addAttribute("GML","spec","0.1b",0);

	xml.pushTag("GML");
	//---------------------
	xml.addTag("tag");
	xml.pushTag("tag");
			
		
		xml.addTag("header");
		xml.pushTag("header");
			
				xml.addValue("client:name", "eyeTagger");
	
		xml.popTag();
	
	
		xml.addTag("drawing");
		xml.pushTag("drawing");
	
			int c = 0;
			for(int k = 0; k < tag.strokes.size(); k++){
				if(  tag.strokes[k].pts.size() <= 0 )continue;
				
				xml.addTag("stroke");
				xml.pushTag("stroke", c);
				xml.setValue("curved", (int)tag.strokes[k].isCurve);
					for( int i = 0; i < tag.strokes[k].pts.size(); i++)
					{
						xml.addTag("pt");
						xml.pushTag("pt",i);
							strokePoint & pt = tag.strokes[k].pts[i]; 
						
							xml.setValue("x",		pt.x);				//(pt.x-rect.x)/rect.width);
							xml.setValue("y",		pt.y);				//(pt.y - rect.y)/rect.height);
							xml.setValue("time",	pt.t-startTime);
						xml.popTag();
					}
				c++;
				xml.popTag();
			}
		
	xml.popTag();
	
	xml.popTag();
	//----------------------
	xml.popTag();
}


string grafIO::saveState(vector <strokeGroup> & tags, string directory, string prefix){
	string timeStr = getTimeAsStr();
	string filename = directory+prefix+"-"+timeStr+".gml";
	saveStateAndFileName(tags, filename);
	
	return filename;
}

void grafIO::saveStateAndFileName(vector <strokeGroup> & tags, string filename, string actualFileName){
	
	ofxXmlSettings xmlOut;
	
	ofRectangle boundingRect = strokeUtils::getBoundingRectFromGroups(tags);
	
	if( tags.size() <= 0 || boundingRect.width == 0 || boundingRect.height == 0){ ///strokes.size() <= 0){
		return;
	}
	
	xmlOut.clear();
	
	xmlOut.addTag("GML");
	//xmlOut.addAttribute("GML","spec","0.1b",0);
	
	xmlOut.pushTag("GML");
	//---------------------
	xmlOut.addTag("tag");
	xmlOut.pushTag("tag");
	
	
	xmlOut.addTag("header");
	xmlOut.pushTag("header");
	
	if( actualFileName != "")xmlOut.setValue("filename", actualFileName);
	
	xmlOut.addValue("client:name", "eyeSaver-003");

	xmlOut.setValue("environment:screenBounds:x", boundingRect.width);
	xmlOut.setValue("environment:screenBounds:y", boundingRect.height);
	
	xmlOut.setValue("environment:origin:x", boundingRect.x);
	xmlOut.setValue("environment:origin:y", boundingRect.y);
	
	xmlOut.popTag();
	
//	printf("SAVING OUT %i tags\n", tags.size());
	
	int count = 0;
	//-----------------------------------------------------------------
	for (int i = 0; i < tags.size(); i++){
	
//		printf("\n\ntags[%i]\n", i );
//		printf("num strokes is %i\n",  tags[i].strokes.size());
//		printf("num pts is %i\n",  tags[i].getTotalNumPoints() );
//
		strokeGroup & tag = tags[i];
		
		tag.updateBoundingRect();
		ofRectangle rect = tag.getBoundingRect();
		if( tag.getTotalNumPoints() <= 0 || rect.width <= 0 || rect.height <= 0 ){
//			printf("continue\n");
			continue;
		}
		
//		printf("getting here?\n");
		
		xmlOut.addTag("drawing");
		xmlOut.pushTag("drawing", count);
		count++;
		
		float startTime = tag.getStartTime();
		
		
		int c = 0;
		for(int k = 0; k < tag.strokes.size(); k++){
			if(  tag.strokes[k].pts.size() <= 0 ){
//				printf("pts.size() is 0\n");
				continue;
			}
			
			xmlOut.addTag("stroke");
			xmlOut.pushTag("stroke", c);
			
			
			if( tag.strokes[k].isCurve ){
				xmlOut.setValue("info:curved", 1);
			}
			
			// 
			//  curved	
			//
			
			//if (tag.strokes[k].isCurve){
//				// let's turn it into points...
//				//;
//				if (tag.strokes[k].pts.size() < 2) continue;
//				
//				ofBeginShape();
//				ofCurveVertex(tag.strokes[k].pts[0].x, tag.strokes[k].pts[0].y);
//				for(int i = 0; i < tag.strokes[k].pts.size(); i++){
//					ofCurveVertex(tag.strokes[k].pts[i].x, tag.strokes[k].pts[i].y);
//				} 
//				ofCurveVertex(tag.strokes[k].pts.back().x, tag.strokes[k].pts.back().y);
//				
//				vector <ofPoint> curvedPointsTurnedIntoStraight = getVertexData();
//				
//				cout << curvedPointsTurnedIntoStraight.size() << endl;
//			
//				ofBeginShape(); // a hack to clear everything ;)
//				ofEndShape();
//				
//				
//				for (int m = 0; m < curvedPointsTurnedIntoStraight.size(); m++){
//					cout << "m = " << m << "," << curvedPointsTurnedIntoStraight[m].x << "  " << curvedPointsTurnedIntoStraight[m].y << endl;
//				}
//				
//				
//				for( int j = 0; j < (tag.strokes[k].pts.size()-1); j++){
//					for (int l = 0; l < 20; l++){
//						
//						cout << j*20+l << endl;
//						xmlOut.addTag("pt");
//						xmlOut.pushTag("pt", j*20+l);
//						strokePoint & pt = tag.strokes[k].pts[j]; 
//						
//						xmlOut.setValue("x",		curvedPointsTurnedIntoStraight[j*20+l].x);				//(pt.x-rect.x)/rect.width);
//						xmlOut.setValue("y",		curvedPointsTurnedIntoStraight[j*20+l].y);				//(pt.y - rect.y)/rect.height);
//						xmlOut.setValue("time",	pt.t-startTime);
//						xmlOut.popTag();	
//					}
//				}
//				 
//			} else {
				for( int j = 0; j < tag.strokes[k].pts.size(); j++){
					
					xmlOut.addTag("pt");
					xmlOut.pushTag("pt",j);
					strokePoint & pt = tag.strokes[k].pts[j]; 
					
					float maxSize = MAX( boundingRect.width, boundingRect.height);
					
					float px = (pt.x - boundingRect.x) / maxSize;
					float py = (pt.y - boundingRect.y) / maxSize;
					
					xmlOut.setValue("x",		px);				//(pt.x-rect.x)/rect.width);
					xmlOut.setValue("y",		py);				//(pt.y - rect.y)/rect.height);
					xmlOut.setValue("time",	pt.t-startTime);
					xmlOut.popTag();
				}
			//}
				
			c++;
			xmlOut.popTag();
		}
		
		xmlOut.popTag();
	

	}
	
	xmlOut.popTag();
	xmlOut.popTag();
	//----------------------
	
	
	xmlOut.saveFile(filename);
	//-----------------------------------------------------------------
}


void grafIO::loadState(vector <strokeGroup> & tags, string filename){
	
	ofxXmlSettings xmlIn;
	
	xmlIn.loadFile(filename);
	
	//tags.clear();
	
	
	xmlIn.pushTag("GML");
	//---------------------
	xmlIn.pushTag("tag");
	xmlIn.pushTag("header");
	
	ofRectangle boundingRect;
	
	boundingRect.width  = xmlIn.getValue("environment:screenBounds:x", 1024.0f);
	boundingRect.height = xmlIn.getValue("environment:screenBounds:y", 768.0f);
	boundingRect.x  = xmlIn.getValue("environment:origin:x", 0.0);
	boundingRect.y = xmlIn.getValue("environment:origin:y", 0.0);
	
	xmlIn.popTag();
	

	int nDrawings = xmlIn.getNumTags("drawing");
	printf("nDrawings is %i\n", nDrawings);
	
	tags.clear();

	//-----------------------------------------------------------------
	for (int i = 0; i < nDrawings; i++){
		tags.push_back(strokeGroup());
		xmlIn.pushTag("drawing", i);

		int nStrokes = xmlIn.getNumTags("stroke");
		
		//cout << "nStrokes " << nStrokes << endl;

		for(int j = 0; j < nStrokes; j++){
				xmlIn.pushTag("stroke", j);
				tags.back().strokes.push_back(stroke());
				
				bool bCurve = xmlIn.getValue("info:curved", false);
				tags.back().strokes.back().setIsCurve(bCurve);
				
				int nPts = xmlIn.getNumTags("pt");
				//cout << "nPts " << nPts << endl;
				
				for( int k = 0; k < nPts; k++){
					
					xmlIn.pushTag("pt", k);
					
					float x = xmlIn.getValue("x",		0.0f);				//(pt.x-rect.x)/rect.width);
					float y = xmlIn.getValue("y",		0.0f);				//(pt.y - rect.y)/rect.height);
					float t = xmlIn.getValue("time",	0.0f);
				
					float maxSize = MAX(boundingRect.width, boundingRect.height);
					
					x *= maxSize;
					y *= maxSize;
					
					x += boundingRect.x;
					y += boundingRect.y;
				
					//printf("drawing %i stroke %i pt %i   x,y,t = (%f, %f, %f) \n", i,j,k,x,y,t);
					tags.back().strokes.back().addPoint(x, y, t, 1.0);
						   
					xmlIn.popTag();
				}
				tags.back().strokes.back().updateBoundingRect();
				
				xmlIn.popTag();
		}
		
		tags.back().updateBoundingRect();
		
		xmlIn.popTag();
		
		
	}
	
	xmlIn.popTag();
	xmlIn.popTag();
	//----------------------
	
	if( tags.size() )tags.push_back(strokeGroup());
	
	printf("get number of groups %i\n", tags.size());
	
	//-----------------------------------------------------------------
}


//void grafIO::loadSavedTag(strokeGroup & tag, string filename)
//{
//	xml.clear();
//	
//	xml.loadFile(filename);
//	
//	xml.pushTag("GML");
//	//-----------------
//		xml.pushTag("tag");
//			
//			xml.pushTag("drawing");
//				
//				xml.pushTag("stroke");
//				
//				int numPts = xml.getNumTags("pt");
//				cout << "tag n pts " << numPts << endl;
//				for( int i = 0; i < numPts-1; i++)
//				{
//					xml.pushTag("pt",i);
//					
//						float x = xml.getValue("x",0.f);
//						float y = xml.getValue("y",0.f);
//						float z = xml.getValue("z",0.f);
//						float t = xml.getValue("time",0.f);
//					
//						bool bnewpt = tag->addNewPoint(ofPoint(x,y,z), -1, -1, t, false);
//						cout << x << " " << y << " " << z << " new point " << bnewpt << endl;
//					
//					xml.popTag();
//				}
//	
//				xml.popTag();
//	
//			xml.popTag();
//	
//		xml.popTag();
//	//-----------------
//	xml.popTag();
//	
//	cout << "tag pts " << tag->pts.size() << endl;
//	tag->setCurrentToLast();
//	tag->calcMinMax();
//	tag->tagname.clear();
//	tag->tagname = filename;
//}


