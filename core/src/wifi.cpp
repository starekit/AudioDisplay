#include "WIFI.h"
using namespace core;
// Wifi::Wifi(){
// 	initNVS();
// 	initWifi();
// }
void Wifi::initNVS(){
	if(nvs_flash_init()!=ESP_OK){
		ESP_LOGE(TAG,"ERROR:init nvs flash");
		return;
	}
}
void Wifi::initWifi(){

	esp_event_loop_create_default();

	// 初始化网络接口
	esp_netif_init();
	esp_netif_create_default_wifi_sta();
	// WiFi配置
	wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
	esp_wifi_init(&cfg);

	esp_event_handler_instance_t instance_any_id;
	// 注册事件处理
	esp_event_handler_instance_register(WIFI_EVENT, 
										ESP_EVENT_ANY_ID,
										&wifiEventHandler, 
										this, 
										&instance_any_id);
	  
	esp_event_handler_instance_register(IP_EVENT, 
										IP_EVENT_STA_GOT_IP,
										&wifiEventHandler, 
										this, 
										&instance_any_id);
}
void Wifi::AP(const string ssid,const string password){
	wifi_config_t wifi_config = {
		.ap = {
			.ssid = "MyESP32AP",
			// .ssid_len = strlen("MyESP32AP"),
			// .password = "password",
			// .max_connection = 4,
			// .authmode = WIFI_AUTH_WPA_WPA2_PSK
		}
	};

	esp_wifi_set_mode(WIFI_MODE_AP);
	esp_wifi_set_config(WIFI_IF_AP,&wifi_config);
	esp_wifi_start();
}
void Wifi::STA(const string ssid,const string password){

	wifi_config_t wifi_config = {};
    strncpy((char*)wifi_config.sta.ssid, ssid.c_str(), sizeof(wifi_config.sta.ssid) - 1);
    wifi_config.sta.ssid[sizeof(wifi_config.sta.ssid) - 1] = '\0';
    
    // if (password && strlen(password.c_str()) > 0) {
        strncpy((char*)wifi_config.sta.password, password.c_str(), sizeof(wifi_config.sta.password) - 1);
        wifi_config.sta.password[sizeof(wifi_config.sta.password) - 1] = '\0';
        wifi_config.sta.threshold.authmode = WIFI_AUTH_WPA2_PSK;
    // } else {
    //     wifi_config.sta.threshold.authmode = WIFI_AUTH_OPEN;
    // }
    
    // 设置其他字段
    wifi_config.sta.scan_method = WIFI_ALL_CHANNEL_SCAN;
    wifi_config.sta.sort_method = WIFI_CONNECT_AP_BY_SIGNAL;
    wifi_config.sta.threshold.rssi = -127;
    
    esp_wifi_set_mode(WIFI_MODE_STA);
    esp_wifi_set_config(WIFI_IF_STA, &wifi_config);
    esp_wifi_start();
    
    ESP_LOGI("WebServer", "WiFi STA mode initialized for SSID: %s", ssid);

	// ESP_LOGV(TAG,"ssid:%s",ssid.c_str());
	// ESP_LOGV(TAG,"ssid:%s",password.c_str());
	// 配置STA模式
	// wifi_config_t wifi_config = {
	// 	.sta = {
	// 		.ssid =*ssid.c_str() ,
	// 		.password =*password.c_str(),
	// 	},
	// };

	// esp_wifi_set_mode(WIFI_MODE_STA);
	// esp_wifi_set_config(WIFI_IF_STA, &wifi_config);
	// esp_wifi_start();

}
void Wifi::Mode(){

}
void Wifi::wifiEventHandler(void* arg, esp_event_base_t event_base,
                            int32_t event_id, void* event_data) {
	
	if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_START) {
		esp_wifi_connect();
	} else if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_DISCONNECTED) {
		esp_wifi_connect();
	} else if (event_base == IP_EVENT && event_id == IP_EVENT_STA_GOT_IP) {
		ip_event_got_ip_t* event = (ip_event_got_ip_t*) event_data;
		ESP_LOGV(TAG,"Got IP: " IPSTR "\n", IP2STR(&event->ip_info.ip));
	}
}

