#ifndef GMLREADER_H
#define GMLREADER_H

#include "ofxXmlSettings.h"

class gmlReader
{
    public:
        gmlReader();
        virtual ~gmlReader();
		
		void loadGML( string filename );
		
	
		ofxXmlSettings	xml;

	protected:
		
			
    private:
};

#endif // GMLUPLOADER_H
