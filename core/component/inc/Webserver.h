#pragma once
#include "WIFI.h"
#include "WebServerRequest.h"

namespace core{

	class WebServer{
		private:
			httpd_handle_t server;
			httpd_config_t config;
			//普通请求处理器
			using RequestHandler=std::function<void(WebServerRequest* request)>;
			std::unordered_map<std::string, RequestHandler> handlers;

			// 文件上传处理器
			using UploadHandler = std::function<void(WebServerRequest* request, std::string filename, size_t index, uint8_t* data, size_t len, bool final)>;
			std::unordered_map<std::string, UploadHandler> uploadHandlers;

			std::unique_ptr<Wifi> wifi=std::make_unique<Wifi>();
			// 静态函数需要访问的静态成员
			static WebServer* instance;
		public:
			WebServer();
			~WebServer();

		private:
			esp_err_t startServer();

			static esp_err_t staticRequestHandler(httpd_req_t *req);       
			static esp_err_t staticUploadHandler(httpd_req_t *req);
			esp_err_t instanceRequestHandler(httpd_req_t *req);
			esp_err_t handleUploadRequest(httpd_req_t *req);
			static void wifi_event_handler(void*arg,esp_event_base_t event_base,
											int32_t event_id,void*event_data);
			
			
			void processMultipartUpload(httpd_req_t *req, UploadHandler handler, WebServerRequest* request);
			//任务队列
			public:
				esp_err_t begin();
				void STA(const std::string ssid,const std::string password);
				void AP(const std::string ssid,const std::string password );
				void on(const char*url, httpd_method_t method,
				std::function<void (WebServerRequest *request)> handleFunction);
				void on(const char* url, 
				std::function<void(WebServerRequest* request)> handleFunction);
				void onNotFound(std::function<void(WebServerRequest*request)>notFoundFunction);
				void onUpload(const char* url, httpd_method_t method, UploadHandler uploadFunction);
		};	


}
