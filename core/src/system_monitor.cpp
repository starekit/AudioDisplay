#include "system_monitor.h"
using namespace core;

void SystemMonitor::initSensor(){
	temperature_sensor_config_t temp_sensor_config= {
		.range_min = 20,
		.range_max = 80,
		.clk_src = TEMPERATURE_SENSOR_CLK_SRC_DEFAULT, 
		.flags = {
			.allow_pd = false                     
		}
	};
    ESP_ERROR_CHECK(temperature_sensor_install(&temp_sensor_config, &temp_sensor));
    ESP_ERROR_CHECK(temperature_sensor_enable(temp_sensor));
}

float SystemMonitor::getCpuTemperature(){
    float temp = 0;
 	ESP_ERROR_CHECK(temperature_sensor_get_celsius(temp_sensor, &temp));
    ESP_LOGV(TAG,"CPU Temperature: %.2f °C\n", temp);
    return temp;
}

void SystemMonitor::updateCpuStats(){
    TaskStatus_t *pxTaskStatusArray;
    UBaseType_t uxArraySize;
    uint32_t ulTotalRunTime;

    uxArraySize = uxTaskGetNumberOfTasks();
    pxTaskStatusArray = (TaskStatus_t *)pvPortMalloc(uxArraySize * sizeof(TaskStatus_t));

	if(pxTaskStatusArray != NULL) {
        uxArraySize = uxTaskGetSystemState(pxTaskStatusArray, uxArraySize, &ulTotalRunTime);
        
        uint32_t currentIdle0 = 0, currentIdle1 = 0;
        
        for(int i = 0; i < uxArraySize; i++) {
            if(strcmp(pxTaskStatusArray[i].pcTaskName, "IDLE0") == 0) {
                currentIdle0 = pxTaskStatusArray[i].ulRunTimeCounter;
            } else if(strcmp(pxTaskStatusArray[i].pcTaskName, "IDLE1") == 0) {
                currentIdle1 = pxTaskStatusArray[i].ulRunTimeCounter;
            }
        }
        
        // 只有在有上次数据时才计算
        if(stats.lastTotal > 0) {
            uint32_t totalDiff = ulTotalRunTime - stats.lastTotal;
            
            if(totalDiff > 0) {
                stats.core0Used = 100.0f * (1.0f - (float)(currentIdle0 - stats.lastIdle0) / (float)totalDiff);
                stats.core1Used = 100.0f * (1.0f - (float)(currentIdle1 - stats.lastIdle1) / (float)totalDiff);
            }
        }
        
        // 更新统计
        stats.lastIdle0 = currentIdle0;
        stats.lastIdle1 = currentIdle1;
        stats.lastTotal = ulTotalRunTime;
        
        vPortFree(pxTaskStatusArray);
	}
}
float SystemMonitor::getCpuUsed(){
	updateCpuStats();
    return (stats.core0Used+stats.core1Used)/2.0f;
}
uint32_t SystemMonitor::getMemoryUsed(){

    return 20;
}

