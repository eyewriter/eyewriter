#include "Poco/Net/HTTPStreamFactory.h"
#include "Poco/Net/HTTPClientSession.h"
#include "Poco/Net/HTTPRequest.h"
#include "Poco/Net/HTTPResponse.h"
#include "Poco/Net/HTMLForm.h"
#include "Poco/URIStreamOpener.h"
#include "Poco/StreamCopier.h"
#include "Poco/URI.h"
#include "Poco/Exception.h"
#include "Poco/Net/FTPStreamFactory.h"
#include "Poco/Net/FTPClientSession.h"
#include "Poco/File.h"
#include "Poco/Path.h"

#include <fstream>
#include <ios>
#include <iostream>

using namespace std;

//tried keeping the session open across multiple file uploads
//but this was not as reliable as creating a new session
//for each upload.

class simpleFtpUpload{

    public:
        simpleFtpUpload(){
            ftpClient   = NULL;
            bVerbose    = false;
            host        = "";
            user        = "";
            pass        = "";
            port        = 0;
            bSetup      = false;
        }

        //------------------------------------------------------------
        ~simpleFtpUpload(){
            endFtpSession();
        }

        //------------------------------------------------------------
        void setVerbose(bool verbose){
            bVerbose = verbose;
        }

        //------------------------------------------------------------
        void setup(string _host, string username, string password, int _port = 21){
            host    = _host;
            user    = username;
            pass    = password;
            port    = _port;
            bSetup  = true;
        }

        //------------------------------------------------------------
        int uploadFile(string fileName, string localFolder, string remoteFolder){
            if( bSetup == false ){
                if( bVerbose )printf("error - you need to call setup first\n");
                return -1;
            }

            int numBytes = 0;

            try{
                startFtpSesssion();

                if( bVerbose )printf("ftp-ing %s\n", fileName.c_str());

                localFolder = ofToDataPath( localFolder );

                //add slashes if they don't exist
				if(localFolder.length() > 0){
                    if( localFolder[localFolder.length()-1] != '/' ){
                        localFolder += "/";
                    }
				}

				if(remoteFolder.length() > 0){
                    if( remoteFolder[remoteFolder.length()-1] != '/' ){
                        remoteFolder += "/";
                    }
				}

				string localPath    = localFolder  + fileName;
				string remotePath   = remoteFolder + fileName;

				ostringstream remoteOSS;
				remoteOSS << remoteFolder << fileName;

                if( bVerbose )printf("localpath is %s\n remotepath is %s\n", localPath.c_str(), remotePath.c_str());

				ftpClient->login(user, pass);
                ftpClient->setFileType(Poco::Net::FTPClientSession::TYPE_BINARY);

				ostream& ftpOStream = ftpClient->beginUpload(remoteOSS.str().c_str());  //how to make it overwrite?

				ifstream localIFStream(localPath.c_str(), ifstream::in | ifstream::binary);
				numBytes = Poco::StreamCopier::copyStream(localIFStream, ftpOStream);
				ftpClient->endUpload();

                endFtpSession();

                if(bVerbose)printf("uploaded %i bytes\n\n", numBytes);

			}
			catch (Poco::Exception& exc)
			{
				cout << exc.displayText() << endl;
				return -1;
			}
			return numBytes;
        }

        //------------------------------------------------------------
        //------------------------------------------------------------

        protected:

        //------------------------------------------------------------
            void startFtpSesssion(){
                endFtpSession();

                ftpClient = new Poco::Net::FTPClientSession(host, port);

            }

        //------------------------------------------------------------
            void endFtpSession(){
                if( ftpClient != NULL ){
                    ftpClient->close();
                    delete ftpClient;
                    ftpClient = NULL;
                }
            }


        bool bVerbose;

        Poco::Net::FTPClientSession* ftpClient;
        string user, host, pass;
        int port;
        bool bSetup;
};
