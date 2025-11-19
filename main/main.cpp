#include "system.h"
#include "core.hpp"

extern "C" void app_main(void){

	while(1){
		// static int counter = 0;
		// if (++counter % 10 == 0) {  // 每10秒打印一次

		// 	// printf("Task Runtime Stats:\n%s\n", cTaskStatsBuffer);
		// }
        vTaskDelay(200 / portTICK_PERIOD_MS);  // 延迟500毫秒
    }
        
}
