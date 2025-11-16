#pragma once
#include "WebServerRequest.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_system.h"
#include "nvs_flash.h"
#include "esp_netif.h"
#include <functional>
#include "wifi.h"
#include <memory>

using namespace std;
namespace core{

	class WebServer{
		private:
			httpd_handle_t server;
			httpd_config_t config;

			std::unordered_map<std::string, std::function<void(WebServerRequest*)>> handlers;

			unique_ptr<Wifi> wifi=make_unique<Wifi>();
			// 静态函数需要访问的静态成员
			static WebServer* instance;
		public:
			WebServer();
			~WebServer();

		private:
			bool startServer();

				static esp_err_t staticRequestHandler(httpd_req_t *req);       

				esp_err_t instanceRequestHandler(httpd_req_t *req);

				static void wifi_event_handler(void*arg,esp_event_base_t event_base,
												int32_t event_id,void*event_data);
			//任务队列
			public:
				bool begin();
				void STA(const string*ssid,const string *password);
				void AP(const string* ssid,const string*password );
				void on(const char*url, httpd_method_t method,
				std::function<void (WebServerRequest *request)> handleFunction);
				void on(const char* url, 
				std::function<void(WebServerRequest* request)> handleFunction);
				void onNotFound(std::function<void(WebServerRequest*request)>notFoundFunction);
		};


}
