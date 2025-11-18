#include "WebServerManage.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "esp_timer.h"
#include "SystemMonitor.h"
using namespace manager;

extern "C" void app_main(void){
	SystemMonitor systemMonitor;
    WebServerManage webServerManage;
    webServerManage.begin();

	while(1){
		// static int counter = 0;
		// if (++counter % 10 == 0) {  // 每10秒打印一次

		// 	// printf("Task Runtime Stats:\n%s\n", cTaskStatsBuffer);
		// }
        vTaskDelay(200 / portTICK_PERIOD_MS);  // 延迟500毫秒
    }
        
}
