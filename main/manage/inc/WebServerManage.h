#pragma once
#include "Webserver.h"
using namespace core;
namespace manager{

    class WebServerManage{

        public:
            WebServerManage();
            ~WebServerManage();

        private:

        public:
            


    };



}

// #pragma once

// #include <Arduino.h>
// #include <core.h>
// #include <AsyncTCP.h>
// #include <WiFi.h>
// #include <ESPAsyncWebServer.h>
// #include <Preferences.h>
// #include <LED.h>
// #include <ArduinoJson.h>
// #include <sensor.h>
// #include <mycpu.h>

// constexpr const char* AP_SSID = "ESP32";
// constexpr const char* AP_PASSWORD="12345678";

// const unsigned char AP_IP[]={192,168,1,0};
// const unsigned char AP_GATEWAY[]={192,168,1,0};
// const unsigned char AP_SUBNET[]={255,255,255,0};

// #define PORT 80

// struct WifiInfo{
//     String ssid;
//     int rssi;
// };

// class WebServers{
//     private:
//         unique_ptr<LED> led=make_unique<LED>();
//         unique_ptr<Sensor> sensor=make_unique<Sensor>();
//         // unique_ptr<HybridCpuMonitor> cpuMonitor=make_unique<HybridCpuMonitor>();
//         String index_html;
		
//         //AP模式默认配置参数
//         unique_ptr<String> ap_ssid=make_unique<String>(AP_SSID);
//         unique_ptr<String>ap_password=make_unique<String>(AP_PASSWORD);
//         IPAddress ap_ip;
//         IPAddress ap_gateway;
//         IPAddress ap_subnet;
		
//         // STA模式默认配置参数
//         String sta_ssid="";
//         String sta_password="";

//         uint8_t port=PORT;

//         unique_ptr<AsyncWebServer> server=make_unique<AsyncWebServer>(port);
//         unique_ptr<Preferences> prefs=make_unique<Preferences>();

//        public:
// 			WebServers();
// 			~WebServers();
//     private:
//         void AP_init();
//         void STA_init();
//         void load_config();  
// 		void read_html(String htmlPath);                          
//         void save_wifi(String ssid,String password); 
//         String get_wifi_scan_options();	
		
//         void wifi_config_server();
//         void index_web_server();
//         //index webserver functions.
//         void index_root(AsyncWebServerRequest *request);
//         void show_temaputer(AsyncWebServerRequest *request);
//         void reboot(AsyncWebServerRequest *request);
//         void set_port();
//         void set_led_color(AsyncWebServerRequest *request);
//         void led_on(AsyncWebServerRequest *request);
//         void led_off(AsyncWebServerRequest *request);
//         void data_request(AsyncWebServerRequest *request);
//         void load_device_info(AsyncWebServerRequest *request);
//         void update_system(AsyncWebServerRequest *request,String filename, size_t index, uint8_t *data, size_t len, bool final);
//         void get_system_setting(AsyncWebServerRequest *request);
        
//         //wifi config serve;r functions.
// 		void config_root(AsyncWebServerRequest *request);
//         void save_config(AsyncWebServerRequest *request);						
//         void not_found(AsyncWebServerRequest *request);
		
	
// 	public:
// 		void begin();



// };