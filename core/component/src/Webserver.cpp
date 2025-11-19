#include "webserver.h"
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
esp_err_t WebServer::begin(){
    return instance->startServer();
}

esp_err_t WebServer::startServer(){
    return httpd_start(&server, &config);
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
void WebServer::onUpload(const char* url, httpd_method_t method, UploadHandler uploadFunction){

        uploadHandlers[url] = uploadFunction;
        
        httpd_uri_t handle = {
            .uri       = url,
            .method    = method,
            .handler   = staticUploadHandler,  // 使用专门的上传处理器
            .user_ctx  = this
        };
        httpd_register_uri_handler(server, &handle);

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
esp_err_t WebServer::staticUploadHandler(httpd_req_t *req){
    if(instance){
        return instance->handleUploadRequest(req);
    }
    return ESP_FAIL;
}
esp_err_t WebServer::handleUploadRequest(httpd_req_t *req){
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
void WebServer::STA(const std::string ssid,const std::string password){
	wifi->STA(ssid,password);
}
void WebServer::AP(const std::string ssid,const std::string password){
	// wifi->AP()

}

void WebServer::processMultipartUpload(httpd_req_t *req, UploadHandler handler, WebServerRequest* request){
    
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