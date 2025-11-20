#include "wifi.h"
#include "esp_netif.h"
#include "lwip/ip_addr.h"
using namespace core;
Wifi::Wifi(){
	esp_event_loop_create_default();
	esp_netif_init();
	

}
void Wifi::registerHandler(){
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
void Wifi::AP(const std::string ssid,const std::string password){

	// 2. 创建 AP 网络接口
	esp_netif_t* ap_netif = esp_netif_create_default_wifi_ap();
	// if (!ap_netif) {
	// 	ESP_LOGE("WiFiAP", "Failed to create default WiFi AP netif");
	// 	return false;
	// }

	  // 3. 设置静态 IP 地址
    esp_netif_ip_info_t ip_info;
    IP4_ADDR(&ip_info.ip, 192, 168, 0, 1);      // AP IP: 192.168.4.1
    IP4_ADDR(&ip_info.gw, 192, 168, 0, 1);      // 网关: 192.168.4.1
    IP4_ADDR(&ip_info.netmask, 255, 255, 255, 0); // 子网掩码: 255.255.255.0

	esp_err_t ret = esp_netif_dhcps_stop(ap_netif); // 先停止 DHCP 服务器
    if (ret != ESP_OK && ret != ESP_ERR_ESP_NETIF_DHCP_ALREADY_STOPPED) {
        ESP_LOGE("WiFiAP", "Failed to stop DHCP server: %s", esp_err_to_name(ret));
        // return false;
    }
    
    ret = esp_netif_set_ip_info(ap_netif, &ip_info);
    if (ret != ESP_OK) {
        ESP_LOGE("WiFiAP", "Failed to set IP info: %s", esp_err_to_name(ret));
        // return false;
    }
    
    // 4. 重新启动 DHCP 服务器（为连接的设备分配 IP）
    ret = esp_netif_dhcps_start(ap_netif);
    if (ret != ESP_OK) {
        ESP_LOGE("WiFiAP", "Failed to start DHCP server: %s", esp_err_to_name(ret));
        // return false;
    }
	wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
	esp_wifi_init(&cfg);

	// 注册事件处理
	registerHandler();


	wifi_config_t wifi_config = {};
	strncpy((char*)wifi_config.ap.ssid,ssid.c_str(),sizeof(wifi_config.ap.ssid)-1);
	wifi_config.ap.ssid[sizeof(wifi_config.ap.ssid)-1]='\0';

    if (!password.empty()) {
        strncpy((char*)wifi_config.ap.password, password.c_str(), sizeof(wifi_config.ap.password) - 1);
        wifi_config.ap.password[sizeof(wifi_config.ap.password) - 1] = '\0';
        wifi_config.ap.authmode = WIFI_AUTH_WPA_WPA2_PSK;  
    } else {
        wifi_config.ap.authmode = WIFI_AUTH_OPEN; 
    }
	
	wifi_config.ap.ssid_len = strlen((char*)wifi_config.ap.ssid);
    wifi_config.ap.channel = 1;                
    wifi_config.ap.max_connection = 4;           
    wifi_config.ap.beacon_interval = 100;      
	
	// esp_err_t ret = esp_wifi_set_mode(WIFI_MODE_AP);
	// if (ret != ESP_OK) {
	// 	ESP_LOGE(TAG, "Failed to set WiFi mode: %s", esp_err_to_name(ret));
	// 	return false;
	// }
	
	// // 4. 应用配置
	// ret = esp_wifi_set_config(WIFI_IF_AP, &wifi_config);
	// if (ret != ESP_OK) {
	// 	ESP_LOGE(TAG, "Failed to set WiFi config: %s", esp_err_to_name(ret));
	// 	return false;
	// }
	
	// // 5. 启动 WiFi
	// ret = esp_wifi_start();
	// if (ret != ESP_OK) {
	// 	ESP_LOGE(TAG, "Failed to start WiFi: %s", esp_err_to_name(ret));
	// 	return false;
	// }
	esp_wifi_set_mode(WIFI_MODE_AP);
	esp_wifi_set_config(WIFI_IF_AP,&wifi_config);
	esp_wifi_start();
	ESP_LOGI("WebServer", "WiFi  mode initialized for SSID: %s", ssid);
}
void Wifi::STA(const std::string ssid,const std::string password){

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

