#pragma once
#include "esp_wifi.h"
#include "nvs_flash.h"
#include "esp_event.h"
namespace core{
    class Wifi{
        public:
            Wifi();
            ~Wifi();
        private:    
        // wifi_event_handle
            void initWifi();

        public:
            void Mode(); 
            void STA();
            void AP();

    };
}