#include "wifi.h"
using namespace core;
Wifi::Wifi(){
	initNVS();
	initWifi();
}
void Wifi::initNVS(){
	if(nvs_flash_init()!=ESP_OK){
		printf("ERROR:init nvs flash");
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

	// 注册事件处理
	  esp_event_handler_instance_register(WIFI_EVENT, ESP_EVENT_ANY_ID,
	                                     &wifiEventHandler, NULL, NULL);
	  esp_event_handler_instance_register(IP_EVENT, IP_EVENT_STA_GOT_IP,
	                                     &wifiEventHandler, NULL, NULL);
}
void Wifi::AP(const string*ssid,const string*password){
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
void Wifi::STA(const string*ssid,const string *password){

	// 配置STA模式
	wifi_config_t wifi_config = {
		.sta = {
			.ssid ="LLL" ,
			.password = "your_PASSWORD",
		},
	};

	esp_wifi_set_mode(WIFI_MODE_STA);
	esp_wifi_set_config(WIFI_IF_STA, &wifi_config);
	esp_wifi_start();

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
		printf("Got IP: " IPSTR "\n", IP2STR(&event->ip_info.ip));
	}
}

