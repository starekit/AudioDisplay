#pragma once 
#include "core_nvs.h"
#include "fft.h"
#include "file_system.h"
#include "system_monitor.h"
#include "sensor.h"
#include "led.h"
#include "webserver_interface.h"

using namespace core;
// SystemMonitor systemMonitor;
// WebServerManage webServerManage;
// webServerManage.begin();


class System{
	//data
	//call back
	//lock

	private:
		std::unique_ptr<Nvs> nvs=std::make_unique<Nvs>();
		std::unique_ptr<FFT> fft=std::make_unique<FFT>();
		std::unique_ptr<FileSystem> fileSystem=std::make_unique<FileSystem>();
		std::unique_ptr<SystemMonitor> systemMonitor=std::make_unique<SystemMonitor>();

		// std::unique_ptr<webserver::WebserverInterface> webserver_=std::make_unique<webserver::WebserverInterface>();
		std::unique_ptr<Led> ledManage=std::make_unique<Led>();
		std::unique_ptr<Sensor> sensorManage=std::make_unique<Sensor>();

	public:
		System();
		~System();
		void begin();


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
// const std::string Ssid="TPLK";
// const std::string Password="Wang5203714";
