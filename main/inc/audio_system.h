#pragma once 
#include "core_nvs.h"
#include "fft.h"
#include "file_system.h"
#include "system_monitor.h"
#include "sensor.h"
#include "led.h"
#include "webserver_interface.h"

using namespace core;
using namespace server;
// SystemMonitor systemMonitor;
// WebServerManage webServerManage;
// webServerManage.begin();


class System{
	//data
	//call back
	//lock

	private:

		static constexpr const char* TAG="System";

		WifiInfoStruct wifiInfo;

		std::unique_ptr<Nvs> nvs_ptr_=std::make_unique<Nvs>();
		std::unique_ptr<FFT> fft_ptr_=std::make_unique<FFT>();

		std::unique_ptr<WebserverInterface> webserver_ptr_;

		std::shared_ptr<Led> led_ptr_=std::make_unique<Led>();
		std::shared_ptr<Sensor> sensor_ptr_=std::make_unique<Sensor>();
		std::shared_ptr<Preference> pref_ptr_=std::make_unique<Preference>();
		std::shared_ptr<FileSystem> file_ptr_=std::make_unique<FileSystem>();
		std::shared_ptr<SystemMonitor> monitor_ptr_=std::make_unique<SystemMonitor>();

	public:
		System(){

		}
		~System(){

		}
		void begin();
	private:
		void selectWebserver();


};
// namespace System


//     void vTaskFunction(void *pvParameters)
// {
//     for(;;)
//     {
//         // 任务主体代码
//         vTaskDelay(pdMS_TO_TICKS(1000)); // 延时1秒
//     }
//     vTaskDelete(NULL); // 删除任务（通常不会执行到这里）
// }

// void vTaskLED(void *pvParameters)
// {
//     while(1)
//     {
//         // 控制LED闪烁
//         vTaskDelay(pdMS_TO_TICKS(500)); // 延时500ms
//     }
// }
//   xTaskCreate(
//         vTaskLED,           // 任务函数
//         "LED Task",         // 任务名称
//         128,               // 堆栈大小
//         NULL,              // 参数
//         1,                 // 优先级
//         NULL               // 任务句柄
//     );

// void WebServerManage::loadConfig(){
//     // prefs->begin("wifi_config");  // 打开名为"wifi_config"的命名空间（只读）
// 	// // staSsid = prefs->read("ssid");
// 	// // staPassword=prefs->read();
// 	// prefs->end();
// #ifdef _DEBUGE_
// 	if (staSsid != "") {
// 		DEBUGE_PRINTF("读取到保存的WiFi: %s\n", sta_ssid.c_str());
// 	} else {
// 		DEBUGE_PRINT("未读取到保存的WiFi配置\n");
// 	}
// #endif

// }
// constexpr const char* AP_SSID = "ESP32";
// constexpr const char* AP_PASSWORD="12345678";

