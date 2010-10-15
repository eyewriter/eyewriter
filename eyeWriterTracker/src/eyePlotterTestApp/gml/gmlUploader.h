#ifndef GMLUPLOADER_H
#define GMLUPLOADER_H

#include "pocoHttpPostForm.h"
#include "ofxXmlSettings.h"


class gmlUploader
{
    public:
        gmlUploader();
        virtual ~gmlUploader();

		void uploadToWeb( string url, ofxXmlSettings gml );
	
    protected:
		
		pocoHttpPostForm submitter;
	
    private:
};

#endif // GMLUPLOADER_H
