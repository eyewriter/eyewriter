#include "gmlUploader.h"


gmlUploader::gmlUploader()
{
    
}

gmlUploader::~gmlUploader()
{
    //dtor
}

void gmlUploader::uploadToWeb( string url, ofxXmlSettings gml  )
{

	submitter.clearFormData();
	submitter.setUrl(url);
	
	string mygml; 
	gml.copyXmlToString(mygml);
	
	//cout << mygml << endl;
	
	printf("addingForm\n");
	
	submitter.addForm("application","gml");
	submitter.addForm("gml",mygml);
	
	printf("submit\n");
	submitter.submit();

	printf("submited :) \n");
	

	if( submitter.wasLastSubmitted() ) cout << "Gml submitted!" << endl;
	else cout << "Not submitted!" << endl;
	
}

