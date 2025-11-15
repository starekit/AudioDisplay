#pragma once
#include <string>
#include <memory>
#include "wifi.h"
#include "Webserver.h"
#include "sensor.h"
#include "JsonDocument.hpp"
#include "esp_wifi.h"
#include "preference.h"

using namespace core;
using namespace std;
namespace manager{

    constexpr const char* AP_SSID = "ESP32";
    constexpr const char* AP_PASSWORD="12345678";

    // #define PORT 80
        

    struct WifiInfo{
        string ssid;
        int rssi;
    };


    class WebServerManage{
        private:
            //  unique_ptr<LED> led=make_unique<LED>();
        	unique_ptr<Sensor> sensor=make_unique<Sensor>();
        	string indexHtml;
		
//         //AP模式默认配置参数
//         IPAddress ap_ip;
//         IPAddress ap_gateway;
//         IPAddress ap_subnet;
		
//         // STA模式默认配置参数
			string staSsid="";
			string staPassword="";

        	// uint8_t port=PORT;
            unique_ptr<Wifi> wifi=make_unique<Wifi>();
			unique_ptr<WebServer> server=make_unique<WebServer>();
            unique_ptr<Preference> prefs=make_unique<Preference>();
        public: 
            WebServerManage();
            ~WebServerManage();

        private:
            void initAP();
            void initSTA();
            void loadConfig();  

    		void readHtml(string htmlPath);                          
            void saveWifi(string ssid,string password); 
        	string getWifiScanOptions();	
		
       		void wifiConfigWebServer();
        	void indexWebServer();
        //index webserver functions.
        	void indexRoot(WebServerRequest *request);
        	void getCPUTemaputer(WebServerRequest *request);
//         	void reboot(AsyncWebServerRequest *request);
//         	void setPort();
//         	void setLEDColor(AsyncWebServerRequest *request);
        	void ledOn(WebServerRequest *request);
        	void ledOff(WebServerRequest *request);
        	void getSensorData(WebServerRequest *request);
        	void loadDeviceInfo(WebServerRequest *request);
        	void updateSystem(WebServerRequest *request,string filename, size_t index, uint8_t *data, size_t len, bool final);
        	void getSystemSetting(WebServerRequest *request);
        
        //wifi config server functions.
			void configRoot(WebServerRequest *request);
        	void saveRonfig(WebServerRequest *request);						
        	void notFound(WebServerRequest *request);

        public:
            void begin();
            void setup();
            


    };



}

