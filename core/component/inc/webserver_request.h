#pragma once
#include "core.hpp"
#include "esp_http_server.h"
namespace core{

class WebServerRequest{
        private:
            QueueHandle_t queue;
            httpd_req_t *req;
            bool responseSent;  // 标记是否已发送响应
        public:
            WebServerRequest(httpd_req_t *request): req(request), responseSent(false){
                ESP_LOGI("WebServerRequest", "Created request handler for URI: %s", req->uri);
            };
            ~WebServerRequest(){}

            WebServerRequest(const WebServerRequest&) = delete;
            WebServerRequest& operator=(const WebServerRequest&) = delete;
        public:
            void send(int statusCode=200,const char* contentType = "text/html", const std::string content="ESP32 Webserver");   
            void sendJson(int statusCode, const std::string jsonString);
            void sendText(int statusCode, const std::string text);
            void sendText(const std::string text);
            void sendHtml(int statusCode, const std::string html);
            void sendHtml(const std::string html);
            void sendError(int statusCode, const std::string message);
            const char*getStatusCodeString(int statusCode);
            bool hasArg(const char *arg);
            std::string arg(const char*arg);

        };  

}