#ifndef POCOHTTPPOSTFORM_H
#define POCOHTTPPOSTFORM_H

#include "ofMain.h"
#include "ofxThread.h"

#include "Poco/Net/HTTPClientSession.h"
#include "Poco/Net/HTTPRequest.h"
#include "Poco/Net/HTTPResponse.h"
#include "Poco/Net/HTMLForm.h"
#include "Poco/StreamCopier.h"
#include "Poco/Path.h"
#include "Poco/URI.h"
#include "Poco/Exception.h"
#include "Poco/BasicEvent.h"
#include "Poco/Delegate.h"


using Poco::Net::HTTPClientSession;
using Poco::Net::HTTPRequest;
using Poco::Net::HTTPResponse;
using Poco::Net::HTMLForm;
using Poco::Net::HTTPMessage;
using Poco::StreamCopier;
using Poco::Path;
using Poco::URI;
using Poco::Exception;
using Poco::BasicEvent;
using Poco::Delegate;


typedef Poco::Timestamp ofTimestamp;



class pocoHttpPostForm : public ofxThread
{
    public:
        pocoHttpPostForm();
        virtual ~pocoHttpPostForm();

		void setUrl( string url );
		void addForm( string name, string value );
		void submit();
		void NonThreadedFunction();
		void clearFormData();
		
		bool wasLastSubmitted();
	
		int timeoutSeconds;
	
    protected:
	
		
		void threadedFunction();
		
		string	url;
		vector<string>	formNames;
		vector<string>	formValues;
	
		bool	bSubmitting;
		bool	bLastFormSubmitted;
	
    private:
};

#endif // GRAFUPLOADER_H
