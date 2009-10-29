#ifndef GRAFIO_H
#define GRAFIO_H

#include "strokeGroup.h"
#include "ofxXmlSettings.h"

#include "appUtils.h"

/* Needs app utils for:

string getTimeAsStr(){
	string sec		= "_" + ofToString( ofGetSeconds() );
	string min		= "_" + ofToString( ofGetMinutes() );
	string hour		= "_" + ofToString( ofGetHours() );
	string day		= "_" + ofToString( ofGetDay() );
	string month	= "_" + ofToString( ofGetMonth() );
	string year		= "_" + ofToString( ofGetYear() );
	
	return year+month+day+hour+min+sec;
}

*/

//NEEDS LOVE

class grafIO
{
    public:
        grafIO();
        virtual ~grafIO();

		//void loadSavedTag(strokeGroup & tag, string filename);
		
		void saveTag(strokeGroup & tag);
		void saveTag(strokeGroup & tag, string filename);
		
		string saveState(vector <strokeGroup> & tags, string directory, string prefix);		
		void saveStateAndFileName(vector <strokeGroup> & tag, string filename, string actualFileName ="");
		void loadState(vector <strokeGroup> & tag, string filename);


		void constructGML(strokeGroup & tag );
	
		ofxXmlSettings	xml;
	
		

	
    protected:
	
	
    private:
};

#endif // GRAFIO_H
