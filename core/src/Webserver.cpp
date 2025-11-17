#include"Webserver.h"
#include <cstring>
using namespace core;
WebServer* WebServer::instance = nullptr;

WebServer::WebServer(){
	config = HTTPD_DEFAULT_CONFIG();
	config.max_uri_handlers = 16;
	config.stack_size = 8192;
	server = nullptr;
	instance=this;
}
WebServer::~WebServer(){
    if (server) {
        httpd_stop(server);
        server = nullptr;
    }
    instance = nullptr;
}
bool WebServer::begin(){
    return instance->startServer();
}

bool WebServer::startServer(){
    
    esp_err_t ret = httpd_start(&server, &config);
    if (ret == ESP_OK) {
        ESP_LOGI("WebServer", "HTTP server started");
        return true;
    } else {
        ESP_LOGE("WebServer", "Failed to start HTTP server: %s", esp_err_to_name(ret));
        return false;
    }
}

void WebServer::on(const char*url, httpd_method_t method,
    std::function<void (WebServerRequest* request)> handleFunction){

    handlers[url] = handleFunction;
    
    // 注册URI处理器
    httpd_uri_t handle = {
        .uri       = url,
        .method    = method,
        .handler   = staticRequestHandler,  // 静态C函数
        .user_ctx  = this  // 传递this指针作为上下文
    };
    
    httpd_register_uri_handler(server, &handle);
}
void WebServer::on(const char* url, std::function<void(WebServerRequest* request)> handleFunction){
    on(url, HTTP_GET, handleFunction);
}
void onNotFound(std::function<void(WebServerRequest*request)>notFoundFunction){
    std::string url="";
    // on(url,HTTP_GET,notFoundFunction);
}

// 静态请求处理器 - 作为C回调
esp_err_t WebServer::staticRequestHandler(httpd_req_t *req){
    if (instance) {
        return instance->instanceRequestHandler(req);
    }
    return ESP_FAIL;
}
 // 实例请求处理器 - 实际调用lambda
esp_err_t WebServer::instanceRequestHandler(httpd_req_t *req){

     std::string uri(req->uri);
        auto it = handlers.find(uri);
        if (it != handlers.end()) {
            WebServerRequest request(req);
            it->second(&request);  // 调用捕获的lambda
            return ESP_OK;
        }
    return ESP_FAIL;
}
void WebServer::STA(const string ssid,const string password){
	wifi->STA(ssid,password);
}
void WebServer::AP(const string ssid,const string password){
	// wifi->AP()

}