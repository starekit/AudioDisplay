#pragma once
#include "esp_wifi.h"
#include "nvs_flash.h"
#include "esp_event.h"
#include <string>
using namespace std;
namespace core{
	class Wifi{
        	public:
            		Wifi();
            		~Wifi();
        	private:
			void initNVS();    
            		static void wifiEventHandler(void* arg, esp_event_base_t event_base,
                                						int32_t event_id, void* event_data);
            		void initWifi();

        	public:
            		void Mode(); 
            		void STA(const string*ssid,const string *password);
            		void AP(const string*ssid,const string*password);

    };
}