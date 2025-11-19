#pragma once
#include "Webserver.h"
#include "Preference.h"
#include "JsonDocument.hpp"
using namespace core;
namespace manage{

    constexpr const char* AP_SSID = "ESP32";
    constexpr const char* AP_PASSWORD="12345678";
    const std::string Ssid="TPLK";
    const std::string Password="Wang5203714";
    // #define PORT 80
        

    struct WifiInfo{
        std::string ssid;
        int rssi;
    };


    class WebServerManage{
        
        private:

            const std::string wifiWebName="wifi.html";
            const std::string indexWebName="index.html";
        	std::string webHtml;
            static constexpr const char* TAG = "WebServerManage";  // 在类内定义 TAG
		
//         //AP模式默认配置参数
//         IPAddress ap_ip;
//         IPAddress ap_gateway;
//         IPAddress ap_subnet;
		
//         // STA模式默认配置参数
			std::string staSsid="";
			std::string staPassword="";
        	// uint8_t port=PORT;
			std::unique_ptr<WebServer> server=std::make_unique<WebServer>();
            std::unique_ptr<Preference> prefs=std::make_unique<Preference>();
        public: 
            WebServerManage();
            ~WebServerManage();

        private:
            void initAP();
            void initSTA();
            void loadConfig();  

    		void readHtml(std::string htmlPath);                          
            void saveWifi(std::string ssid,std::string password); 
        	std::string getWifiScanOptions();	
		
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
        	void updateSystem(WebServerRequest *request,std::string filename, size_t index, uint8_t *data, size_t len, bool final);
        	void getSystemSetting(WebServerRequest *request);
        
        //wifi config server functions.
			void configRoot(WebServerRequest *request);
        	void saveConfig(WebServerRequest *request);						
        	void notFound(WebServerRequest *request);

        public:
            esp_err_t begin();
            void setup();
            


    };



}

