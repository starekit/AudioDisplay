#pragma once
#include "core.hpp"
#include "webserver.h"
#include "preference.h"
#include "json_document.hpp"
using namespace core;

struct WifiInfo{
	std::string ssid;
	int rssi;
};


class WebserverInterface{

	protected:
		std::string index_html_;
		
		std::string ssid_;
		std::string password_;

		std::unique_ptr<WebServer> server=std::make_unique<WebServer>();
		std::unique_ptr<Preference> prefs=std::make_unique<Preference>();
	public:
		virtual ~WebserverInterface()=default;
		virtual void begin()=0;
	protected:
		virtual void root(WebServerRequest *request)=0;
		virtual void notFound(WebServerRequest *request)=0;
		void readHtml(std::string htmlName){
			// index_html_=Fils.read(htmlName)

		}
};