#pragma once
#include "core.hpp"
#include "esp_wifi.h"

namespace core{
	typedef struct {
		char ssid[33];
		int8_t rssi;
		uint8_t channel;
		wifi_auth_mode_t authmode;
	} wifi_ap_info_t;

	class Wifi{
		private:
			static constexpr const char* TAG = "Wifi";
			
			wifi_init_config_t cfg;
			esp_event_handler_instance_t instance_any_id;

			wifi_scan_config_t scan_config;
        public:
			Wifi();
			~Wifi(){
			}
        private:
            static void wifiEventHandler(void* arg, esp_event_base_t event_base,
                                			int32_t event_id, void* event_data);
			
			
			static int compareAPByRssi(const void *a, const void *b);
			void registerHandler();
			void APConfig(const std::string ssid,const std::string password);
			void STAConfig(const std::string ssid,const std::string password);
			void ModeConfig();		
		public:
			void Mode(); 
			void STA(const std::string ssid,const std::string  password);
			void AP(const std::string ssid,const std::string password);
			void scan();
    };
}