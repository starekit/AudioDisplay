#include"Webserver.h"
#include <cstring>
using namespace core;
WebServer* WebServer::instance = nullptr;

WebServer::WebServer(){
    nvs_init();

    config = HTTPD_DEFAULT_CONFIG();
    config.max_uri_handlers = 16;
    config.stack_size = 8192;
    server = NULL;
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
    return instance->server_start();
}

bool WebServer::server_start(){
    
    esp_err_t ret = httpd_start(&server, &config);
    if (ret == ESP_OK) {
        ESP_LOGI("WebServer", "HTTP server started");
        return true;
    } else {
        ESP_LOGE("WebServer", "Failed to start HTTP server: %s", esp_err_to_name(ret));
        return false;
    }
}
void WebServer::sta_init(const char* ssid, const char* password){
    esp_event_loop_create_default();
    
    // 初始化网络接口
    esp_netif_init();
    esp_netif_create_default_wifi_sta();

    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    esp_wifi_init(&cfg);

    // 注册WiFi事件处理器
    esp_event_handler_instance_t instance_any_id;
        esp_event_handler_instance_register(WIFI_EVENT,
                                        ESP_EVENT_ANY_ID,
                                        &WebServer::wifi_event_handler,
                                        this,
                                        &instance_any_id);

    // WiFi配置
    wifi_config_t wifi_config = {};
    strncpy((char*)wifi_config.sta.ssid, ssid, sizeof(wifi_config.sta.ssid) - 1);
    wifi_config.sta.ssid[sizeof(wifi_config.sta.ssid) - 1] = '\0';
    
    if (password && strlen(password) > 0) {
        strncpy((char*)wifi_config.sta.password, password, sizeof(wifi_config.sta.password) - 1);
        wifi_config.sta.password[sizeof(wifi_config.sta.password) - 1] = '\0';
        wifi_config.sta.threshold.authmode = WIFI_AUTH_WPA2_PSK;
    } else {
        wifi_config.sta.threshold.authmode = WIFI_AUTH_OPEN;
    }
    
    // 设置其他字段
    wifi_config.sta.scan_method = WIFI_ALL_CHANNEL_SCAN;
    wifi_config.sta.sort_method = WIFI_CONNECT_AP_BY_SIGNAL;
    wifi_config.sta.threshold.rssi = -127;
    
    esp_wifi_set_mode(WIFI_MODE_STA);
    esp_wifi_set_config(WIFI_IF_STA, &wifi_config);
    esp_wifi_start();
    
    ESP_LOGI("WebServer", "WiFi STA mode initialized for SSID: %s", ssid);
}
void WebServer::nvs_init(){
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);
}       
void WebServer::wifi_event_handler(void*arg,esp_event_base_t event_base,int32_t event_id,void*event_data){
    WebServer*server=static_cast<WebServer*>(arg);
    if(!server) return;
    if(event_base==WIFI_EVENT){
        switch (event_id) {
            case WIFI_EVENT_STA_START:
                ESP_LOGI("WebServer", "WiFi STA start");
                esp_wifi_connect();
                break;
            case WIFI_EVENT_STA_CONNECTED:
                ESP_LOGI("WebServer", "WiFi connected");
                break;
            case WIFI_EVENT_STA_DISCONNECTED:
                ESP_LOGI("WebServer", "WiFi disconnected, reconnecting...");
                esp_wifi_connect();
                break;
        
            default:
                ESP_LOGI("ERROR!","wifi event handler");
                break;
            }
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
void WebServer::STA(const char *ssid,const char* password){
    sta_init(ssid,password);
}
void WebServer::AP(){

}