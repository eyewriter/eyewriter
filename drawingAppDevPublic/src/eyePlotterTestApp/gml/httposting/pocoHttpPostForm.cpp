#include "pocoHttpPostForm.h"


pocoHttpPostForm::pocoHttpPostForm()
{
	bLastFormSubmitted	= false;
	bSubmitting			= false;
	timeoutSeconds		= 40;		// default timeout

}

pocoHttpPostForm::~pocoHttpPostForm()
{
    //dtor
}

void pocoHttpPostForm::setUrl( string surl )
{
	url.clear();
	url = surl;
}

void pocoHttpPostForm::submit()
{

	bSubmitting = true;
		NonThreadedFunction();
	bSubmitting = false;

}

void pocoHttpPostForm::clearFormData()
{
	lock();
		formNames.clear();
		formValues.clear();
	unlock();
}

void pocoHttpPostForm::addForm( string name, string value )
{
	formNames.push_back(name);
	formValues.push_back(value);

}

void pocoHttpPostForm::threadedFunction(){

}

void pocoHttpPostForm::NonThreadedFunction()
{
			try
			{
				cout << "trying to submit" << endl;

				bLastFormSubmitted = false;

				URI uri( url );
				std::string path(uri.getPathAndQuery());
				if (path.empty()) path = "/";

				HTTPClientSession session(uri.getHost(), uri.getPort());
				HTTPRequest req(HTTPRequest::HTTP_POST, path, HTTPMessage::HTTP_1_1);

				session.setTimeout(Poco::Timespan(timeoutSeconds,0));

				// create the form data to send
				HTMLForm pocoForm(HTMLForm::ENCODING_URL);

				// form values
				for(unsigned int i=0; i< formNames.size(); i++){
					const std::string name = formNames[i].c_str();
					const std::string val  = formValues[i].c_str();
					pocoForm.set(name, val);
				}

				pocoForm.prepareSubmit(req);

				printf("pocoForm writing\n");

				try{
				    pocoForm.write(session.sendRequest(req));

                    //HTTPResponse res;
                    //istream& rs = session.receiveResponse(res);
                    //cout << "rs " << res.getReason() << endl;

                    //if( res.getStatus() == 200 )

				}catch (Exception& exc){
                    printf("pocoForm write error \n");
				}

                bLastFormSubmitted = true;
				bSubmitting = false;

			}catch (Exception& exc){
				cout << "Exception thrown!" << endl;
				// ofxHttpEvents.notifyNewError("time out ");
			   // if(verbose) std::cerr << exc.displayText() << std::endl;
			}

}


bool pocoHttpPostForm::wasLastSubmitted(){
	return true;
}

