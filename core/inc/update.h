#pragma once
#include "upload.h"
#include "fileSystem.hpp"
#include "Webserver.h"
#include <memory>
namespace debuge{

	const string Ssid="TPLK";
	const string Password="Wang5203714";
	

    class Update{
		private:
			std::unique_ptr<core::FileSystem> fileSystem=make_unique<core::FileSystem>();
			std::unique_ptr<core::WebServer> webServer=make_unique<core::WebServer>();
		public:
            Update(){
				    webServer->STA(Ssid,Password);
					webServer->begin();
					webServer->on("/", HTTP_GET, [](core::WebServerRequest* req){
						req->send(200, "text/html", uploadHTML.c_str());
					});
			};
			~Update();
			void updateWeb(const string *fileName,const string*file);

            void updateSystem();
    
    
    };  

}