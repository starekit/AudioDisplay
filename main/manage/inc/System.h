#pragma once 
#include "coreNvs.h"
#include "fft.h"
#include "FileSystem.h"
#include "SystemMonitor.h"
#include "WebServerManage.h"
#include "LedManage.h"
#include "SensorManage.h"

using namespace core;
using namespace manage;
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


		std::unique_ptr<LedManage> ledManage=std::make_unique<LedManage>();
		std::unique_ptr<SensorManage> sensorManage=std::make_unique<SensorManage>();
		std::unique_ptr<WebServerManage> webServerManage=std::make_unique<WebServerManage>(); 




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

