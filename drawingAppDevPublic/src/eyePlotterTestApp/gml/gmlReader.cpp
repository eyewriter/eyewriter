#include "gmlReader.h"


gmlReader::gmlReader()
{
    
}

gmlReader::~gmlReader()
{
    //dtor
}

void gmlReader::loadGML( string filename )
{
	
	xml.clear();
	
	if( xml.loadFile(filename) ) cout << "Opened file" << filename << endl;
	else{
		cout << "Could not open file!" << endl;
		return;
	}
	
	
	if( !xml.pushTag("GML") ) return;

	if( !xml.pushTag("tag") ) return;
	
	
	//------- header
	if( xml.pushTag("header") )
	{
		
			//------- environment attributes
			if( xml.pushTag("environment") )
			{
				
				if( xml.pushTag("offset") )
				{
					xml.getValue("x",0.f);
					xml.getValue("y",0.f);
					xml.getValue("z",0.f);
					
					xml.popTag();
				}
				
				if( xml.pushTag("rotation") )
				{
					xml.getValue("x",0.f);
					xml.getValue("y",0.f);
					xml.getValue("z",0.f);
					
					xml.popTag();
				}
				
				
				if( xml.pushTag("up") )
				{
					xml.getValue("x",0.f);
					xml.getValue("y",0.f);
					xml.getValue("z",0.f);
					
					xml.popTag();
				}
				
				if( xml.pushTag("screenBounds") )
				{
					xml.getValue("x",0.f);
					xml.getValue("y",0.f);
					xml.getValue("z",0.f);
					
					xml.popTag();
				}
				
				if( xml.pushTag("origin") )
				{
					xml.getValue("x",0.f);
					xml.getValue("y",0.f);
					xml.getValue("z",0.f);
					
					xml.popTag();
				}
				
				if( xml.pushTag("realScale") )
				{
					xml.getValue("x",0.f);
					xml.getValue("y",0.f);
					xml.getValue("z",0.f);
					xml.getValue("unit","cm");
					xml.popTag();
				}
				
				
				
				xml.popTag();
			}
		
		
			//------ client
			if( xml.pushTag("client") )
			{
			
				xml.getValue("name","noname");
				xml.getValue("version","0.0");
				xml.getValue("keywords","gml");
				xml.getValue("uniqueKey","");
				xml.getValue("ip","");
				xml.getValue("time",0);
				
				if( xml.pushTag("location") )
				{
					xml.getValue("lat",0.0f);
					xml.getValue("lon",0.0f);
				}
				
				xml.popTag();
			}
				
		xml.popTag();
	}
	
	//------- drawing
	if( xml.pushTag("drawing") )
	{
	
		
		int nStrokes = xml.getNumTags("stroke");
		
		
		//------- strokes
		for( int i = 0; i < nStrokes; i++)
		{
						
			xml.pushTag("stroke",i);
				
			//------- brush
			if( xml.pushTag("brush") )
			{
				xml.getValue("mode",0);
				xml.getValue("uniqueStyleID","");
				xml.getValue("spec","");
				xml.getValue("width",1.0);
				xml.getValue("speedToWidthRatio", 1.0);
				xml.getValue("dripAmnt", 1.0);
				xml.getValue("dripSpeed", 1.0);
				xml.getValue("layerAbsolute", 0);
				
				if( xml.pushTag("color" ) )
				{
					xml.getValue("r",255.0);
					xml.getValue("g",255.0);
					xml.getValue("b",255.0);
					xml.getValue("a",255.0);
					xml.popTag();
				}
				
				if( xml.pushTag("dripVecRelativeToUp" ) )
				{
					xml.getValue("x",0.0);
					xml.getValue("y",1.0);
					xml.getValue("z",0.0);
					xml.popTag();
				}
				
				xml.popTag();
			}

			//-------- optional info
			if( xml.pushTag("info") )
			{
				// get custom stuff here like: <curved>true</curved>
				xml.popTag();
			}
			
			int nPts = xml.getNumTags("pt");
				
				for( int j = 0; j < nPts; j++)
				{
					xml.pushTag("pt",j);
						xml.getValue("x",0.f);
						xml.getValue("y",0.f);
						xml.getValue("z",0.f);
						xml.getValue("time",0.f);
					xml.popTag();
				}
			
			xml.popTag();
			
		}
		
		xml.popTag();
	}
	

	xml.popTag(); // tag
	xml.popTag(); // GML
	
}

