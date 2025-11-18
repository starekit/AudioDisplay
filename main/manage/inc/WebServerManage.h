#pragma once
#include <string>
#include <memory>
#include "Wifi.h"
#include "Webserver.h"
#include "FileSystem.h"
#include "Sensor.h"
#include "JsonDocument.hpp"
#include "esp_wifi.h"
#include "Preference.h"
#include "SystemMonitor.h"

using namespace core;
using namespace std;
namespace manager{

    constexpr const char* AP_SSID = "ESP32";
    constexpr const char* AP_PASSWORD="12345678";
    const string Ssid="TPLK";
    const string Password="Wang5203714";
    // #define PORT 80
        

    struct WifiInfo{
        string ssid;
        int rssi;
    };


    class WebServerManage{
        
        private:

            const string wifiWebName="wifi.html";
            const string indexWebName="index.html";
        	string webHtml;
            static constexpr const char* TAG = "WebServerManage";  // 在类内定义 TAG
		
//         //AP模式默认配置参数
//         IPAddress ap_ip;
//         IPAddress ap_gateway;
//         IPAddress ap_subnet;
		
//         // STA模式默认配置参数
			string staSsid="";
			string staPassword="";
            unique_ptr<SystemMonitor> systemMonitor=make_unique<SystemMonitor>();
            unique_ptr<Sensor> sensor=make_unique<Sensor>();
        	// uint8_t port=PORT;
            unique_ptr<FileSystem> fileSystem=make_unique<FileSystem>();
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
        	void reboot(WebServerRequest *request);
//         	void setPort();
        	void setLEDColor(WebServerRequest *request);
        	void ledOn(WebServerRequest *request);
        	void ledOff(WebServerRequest *request);
        	void getSensorData(WebServerRequest *request);
        	void loadDeviceInfo(WebServerRequest *request);
        	void updateSystem(WebServerRequest *request,string filename, size_t index, uint8_t *data, size_t len, bool final);
        	void getSystemSetting(WebServerRequest *request);
        
        //wifi config server functions.
			void configRoot(WebServerRequest *request);
        	void saveConfig(WebServerRequest *request);						
        	void notFound(WebServerRequest *request);

        public:
            void begin();
            void setup();
            


    };



}

