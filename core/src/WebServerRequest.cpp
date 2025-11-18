#include"WebServerRequest.h"
using namespace core;


void WebServerRequest::send(int statusCode,const char* contentType, const string* content){
    // if(responseSent){
    //     ESP_LOGW("WebServer", "Response already sent for URI: %s", req->uri);
    //     return;
    // }
    // httpd_resp_set_status(req, getStatusCodeString(statusCode));
    // httpd_resp_set_type(req, contentType);
    
    // esp_err_t ret = httpd_resp_send(req, content, HTTPD_RESP_USE_STRLEN);
    // if (ret == ESP_OK) {
    //     responseSent = true;
    // } else {
    //     ESP_LOGE("WebServer", "Failed to send response: %s", esp_err_to_name(ret));
    // }


    if(responseSent){
            ESP_LOGW("WebServerRequest", "Response already sent for URI: %s", req->uri);
            return;
        }
        
        httpd_resp_set_status(req, getStatusCodeString(statusCode));
        httpd_resp_set_type(req, contentType);
        
        esp_err_t ret = httpd_resp_send(req, content->c_str(), HTTPD_RESP_USE_STRLEN);
        if (ret == ESP_OK) {
            responseSent = true;
            ESP_LOGI("WebServerRequest", "Response sent successfully for URI: %s", req->uri);
        } else {
            ESP_LOGE("WebServerRequest", "Failed to send response for URI %s: %s", 
                     req->uri, esp_err_to_name(ret));
        }










}

void WebServerRequest::sendJson(int statusCode, const string* jsonString){
    send(statusCode, "application/json", jsonString);
}

void WebServerRequest::sendText(int statusCode, const string* text) {
    send(statusCode, "text/plain", text);
}

void WebServerRequest::sendText(const string* text) {
    sendText(200, text);
}

// 发送HTML
void WebServerRequest::sendHtml(int statusCode, const string* html) {
    send(statusCode, "text/html", html);
}

void WebServerRequest::sendHtml(const string* html) {
    sendHtml(200, html);
}

// 发送错误状态
// void WebServerRequest::sendError(int statusCode, const string* message) {
//     const char* defaultMessages[] = {
//         "400 Bad Request", "401 Unauthorized", "403 Forbidden", 
//         "404 Not Found", "500 Internal Server Error"
//     };
    
//     const char* errorMsg = message->c_str();
//     if (!errorMsg) {
//         // 提供默认错误消息
//         if (statusCode >= 400 && statusCode < 500) {
//             errorMsg = defaultMessages[statusCode - 400];
//         } else {
//             errorMsg = "Error";
//         }
//     }
    
//     char errorHtml[512];
//     snprintf(errorHtml, sizeof(errorHtml),
//         "<html><head><title>Error %d</title></head>"
//         "<body><h1>%d %s</h1></body></html>",
//         statusCode, statusCode, errorMsg);
        
//     sendHtml(statusCode, errorHtml);
// }
const char* WebServerRequest::getStatusCodeString(int statusCode) {
    switch(statusCode) {
        case 200: return "200 OK";
        case 404: return "404 Not Found";
        case 500: return "500 Internal Server Error";
        default: return "200 OK";
    }
}