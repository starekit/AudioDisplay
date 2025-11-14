#pragma once

#include <stdio.h>
#include <iostream>
#include <string.h>
#include "sdkconfig.h"
#include "esp_event.h"
#include "esp_log.h"
#include "esp_netif.h"
#include "esp_http_server.h"

namespace core{

class WebServerRequest{
        private:
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
            void send(int statusCode=200,const char* contentType = "text/html", const char* content = "");   
            void sendJson(int statusCode, const char* jsonString);
            void sendText(int statusCode, const char* text);
            void sendText(const char* text);
            void sendHtml(int statusCode, const char* html);
            void sendHtml(const char* html);
            void sendError(int statusCode, const char* message = nullptr);
            const char*getStatusCodeString(int statusCode);


        };  

}