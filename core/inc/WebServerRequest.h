#pragma once

#include <stdio.h>
#include <iostream>
#include <string.h>
#include <string>
#include "sdkconfig.h"
#include "esp_event.h"
#include "esp_log.h"
#include "esp_netif.h"
#include "esp_http_server.h"
#include "freertos/queue.h"
using namespace std;
namespace core{

//     void vTaskFunction(void *pvParameters)
// {
//     for(;;)
//     {
//         // 任务主体代码
//         vTaskDelay(pdMS_TO_TICKS(1000)); // 延时1秒
//     }
//     vTaskDelete(NULL); // 删除任务（通常不会执行到这里）
// }

// void vTaskLED(void *pvParameters)
// {
//     while(1)
//     {
//         // 控制LED闪烁
//         vTaskDelay(pdMS_TO_TICKS(500)); // 延时500ms
//     }
// }
//   xTaskCreate(
//         vTaskLED,           // 任务函数
//         "LED Task",         // 任务名称
//         128,               // 堆栈大小
//         NULL,              // 参数
//         1,                 // 优先级
//         NULL               // 任务句柄
//     );


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
            void send(int statusCode=200,const char* contentType = "text/html", const string* content=nullptr);   
            void sendJson(int statusCode, const string* jsonString);
            void sendText(int statusCode, const string* text);
            void sendText(const string* text);
            void sendHtml(int statusCode, const string* html);
            void sendHtml(const string* html);
            void sendError(int statusCode, const string* message = nullptr);
            const char*getStatusCodeString(int statusCode);


        };  

}