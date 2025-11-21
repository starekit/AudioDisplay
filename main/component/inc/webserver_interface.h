#pragma once
#include "core.hpp"
#include "wifi.hpp"
#include "config.h"
#include "webserver_request.h"
#include "file_system.h"
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
		private:
			httpd_handle_t server_;
			httpd_config_t config_;
		
			//普通请求处理器
			using RequestHandler=std::function<void(WebServerRequest* request)>;
			std::unordered_map<std::string, RequestHandler> handlers;

			// 文件上传处理器
			using UploadHandler = std::function<void(WebServerRequest* request, std::string filename, size_t index, uint8_t* data, size_t len, bool final)>;
			std::unordered_map<std::string, UploadHandler> uploadHandlers;

			static constexpr const char* TAG="WebserverInterface:";
		public:
			static WebserverInterface *webserver_instance_;

		protected:
	
			std::string index_html_;
			
			std::unique_ptr<Wifi> wifi_ptr_=std::make_unique<Wifi>();
			std::shared_ptr<Preference> prefs_ptr_=nullptr;
			std::shared_ptr<FileSystem> file_ptr_=nullptr;
	
			void initInterface(std::shared_ptr<Preference> prefs,std::shared_ptr<FileSystem>file){
				if(prefs==nullptr||file==nullptr){
					ESP_LOGE(TAG,"初始化网络接口类失败:prefs或者file为空");
				}
				prefs_ptr_=prefs;
				file_ptr_=file;
			}

			static esp_err_t staticRequestHandler(httpd_req_t *req){
				if (webserver_instance_) {
					return webserver_instance_->instanceRequestHandler(req);
				}
				return ESP_FAIL;
			}
			static esp_err_t staticUploadHandler(httpd_req_t *req){
				if(webserver_instance_){
					return webserver_instance_->handleUploadRequest(req);
				}
				return ESP_FAIL;
			}
			esp_err_t instanceRequestHandler(httpd_req_t *req){
				std::string uri(req->uri);
				auto it = handlers.find(uri);
				if (it != handlers.end()) {
					WebServerRequest request(req);
					it->second(&request);  // 调用捕获的lambda
					return ESP_OK;
				}
				return ESP_FAIL;
			}
			esp_err_t handleUploadRequest(httpd_req_t *req){
				std::string uri(req->uri);
				auto it = uploadHandlers.find(uri);
				if(it != uploadHandlers.end()){
					UploadHandler handler = it->second;
					WebServerRequest request(req);
					// 解析 multipart/form-data
					processMultipartUpload(req, handler, &request);
					return ESP_OK;
				}
				return ESP_FAIL;

			}
			void processMultipartUpload(httpd_req_t *req, UploadHandler handler, WebServerRequest* request){

				char boundary[128];
				char buf[1024];
				int received;
				std::string filename;
				bool in_file_section = false;
				size_t file_data_start = 0;
				size_t total_received = 0;

				// 获取 Content-Type 并解析边界
				size_t content_type_len = httpd_req_get_hdr_value_len(req, "Content-Type");
				if (content_type_len > 0) {
					char content_type[content_type_len + 1];
					httpd_req_get_hdr_value_str(req, "Content-Type", content_type, sizeof(content_type));

					// 解析 boundary
					char* bound_start = strstr(content_type, "boundary=");
					if (bound_start) {
						snprintf(boundary, sizeof(boundary), "--%s", bound_start + 9);
					}
				}
			
				// 创建 WebServerRequest 包装器
				WebServerRequest uploadRequest(req);

				// 分块接收数据
				while ((received = httpd_req_recv(req, buf, sizeof(buf))) > 0) {
					if (!in_file_section) {
						// 查找文件数据的开始
						char* file_start = strstr(buf, "\r\n\r\n");
						if (file_start) {
							in_file_section = true;
							file_data_start = file_start - buf + 4;

							// 解析文件名
							char* name_start = strstr(buf, "filename=\"");
							if (name_start) {
								char* name_end = strstr(name_start + 10, "\"");
								if (name_end) {
									filename = std::string(name_start + 10, name_end - name_start - 10);
								}
							}
							
							// 调用回调，开始上传
							handler(request, filename, total_received, (uint8_t*)(buf + file_data_start), received - file_data_start, false);
							total_received += (received - file_data_start);
						}
					} else {
						// 检查是否到达边界（文件结束）
						char* bound_pos = strstr(buf, boundary);
						if (bound_pos) {
							// 写入边界之前的数据
							size_t data_len = bound_pos - buf - 2; // 减去前面的 \r\n
						if (data_len > 0) {
							handler(request, filename, total_received, (uint8_t*)buf, data_len, false);
							total_received += data_len;
						}

						// 最终回调
						handler(request, filename, total_received, nullptr, 0, true);
						break;
					} else {
						// 普通数据块
						handler(request, filename, total_received, (uint8_t*)buf, received, false);
						total_received += received;
					}
				}
			}

				if (received == 0 && in_file_section) {
					// 正常结束，没有找到边界但数据接收完成
					handler(request, filename, total_received, nullptr, 0, true);
				}

				httpd_resp_sendstr(req, "{\"status\":\"success\"}");
			}
			

		public:
			virtual ~WebserverInterface()=default;
			static std::unique_ptr<WebserverInterface> createWifiserver(std::shared_ptr<Preference> prefs,std::shared_ptr<FileSystem>file);
			static std::unique_ptr<WebserverInterface> createIndexserver(std::shared_ptr<Preference> prefs,std::shared_ptr<FileSystem>file,std::shared_ptr<SystemMonitor> systemMonitor);
		protected:
			virtual void begin()=0;
			virtual void root(WebServerRequest *request)=0;
			virtual void notFound(WebServerRequest *request)=0;
			
			void startServer(){
				if(webserver_instance_==nullptr){
					config_ = HTTPD_DEFAULT_CONFIG();
					config_.max_uri_handlers = 16;
					config_.stack_size = 8192;
					webserver_instance_=this;
					
				
				}else{
					ESP_LOGE(TAG,"静态接口初始化异常");
				}
				httpd_start(&server_, &config_);
			}
			void endServer(){
				if(server_){
					httpd_stop(server_);
					webserver_instance_=nullptr;
				}
			}
			void readHtml(std::string htmlName){
				index_html_=file_ptr_->read(&htmlName);
			}
			void on(const char*url, httpd_method_t method,std::function<void (WebServerRequest *request)> handleFunction){
				if(webserver_instance_==nullptr){
					ESP_LOGE(TAG,"this 指针为空!");
				}

				handlers[url] = handleFunction;
				// 注册URI处理器
				httpd_uri_t handle = {
					.uri       = url,
					.method    = method,
					.handler   = staticRequestHandler,  // 静态C函数
					.user_ctx  = this  // 传递this指针作为上下文
				};
			
				
				httpd_register_uri_handler(server_, &handle);
			}
			void on(const char* url, std::function<void(WebServerRequest* request)> handleFunction){
				on(url, HTTP_GET, handleFunction);
			}
			void onNotFound(std::function<void(WebServerRequest*request)> notFoundFunction);
			void onUpload(const char* url, httpd_method_t method, UploadHandler uploadFunction){
				uploadHandlers[url] = uploadFunction;
				httpd_uri_t handle = {
					.uri       = url,
					.method    = method,
					.handler   = staticUploadHandler,  // 使用专门的上传处理器
					.user_ctx  = this
				};
				httpd_register_uri_handler(server_, &handle);
			}

			
	
	};
}
