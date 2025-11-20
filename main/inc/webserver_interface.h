#pragma once
#include "core.hpp"
#include "file_system.h"
#include "webserver.h"
#include "preference.h"
#include "json_document.hpp"
#include "system_monitor.h"
using namespace core;
namespace server{
	struct WifiInfo{
		std::string ssid;
		int rssi;
	};


	class WebserverInterface{

		protected:
			std::string index_html_;
			
			std::string ssid_;
			std::string password_;

			std::shared_ptr<Preference> prefs_ptr_=nullptr;
			std::shared_ptr<FileSystem> file_ptr_=nullptr;

			std::unique_ptr<WebServer> server=std::make_unique<WebServer>();
		public:
			virtual ~WebserverInterface()=default;
			static std::unique_ptr<WebserverInterface> createWifiserver(std::shared_ptr<Preference> prefs,std::shared_ptr<FileSystem>file);
			static std::unique_ptr<WebserverInterface> createIndexserver(std::shared_ptr<Preference> prefs,std::shared_ptr<FileSystem>file,std::shared_ptr<SystemMonitor> systemMonitor);
		protected:
			virtual void begin()=0;
			virtual void root(WebServerRequest *request)=0;
			virtual void notFound(WebServerRequest *request)=0;
			void readHtml(std::string htmlName){
				index_html_=file_ptr_->read(&htmlName);
			}
	};
}
