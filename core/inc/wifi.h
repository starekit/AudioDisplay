#pragma once
#include "core.hpp"
#include "esp_wifi.h"

namespace core{
	class Wifi{
		private:
			static constexpr const char* TAG = "Wifi";
			
			esp_event_handler_instance_t instance_any_id;
        public:
			Wifi();
			~Wifi(){
			}
        private:
            static void wifiEventHandler(void* arg, esp_event_base_t event_base,
                                			int32_t event_id, void* event_data);

			void registerHandler();
			void APConfig(const std::string ssid,const std::string password);

        public:
			void Mode(); 
			void STA(const std::string ssid,const std::string  password);
			void AP(const std::string ssid,const std::string password);

    };
}