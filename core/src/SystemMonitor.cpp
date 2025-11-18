#include "SystemMonitor.h"
using namespace core;

void SystemMonitor::initSensor(){
    temp_sensor_config_t temp_sensor = TSENS_CONFIG_DEFAULT();
    temp_sensor_set_config(temp_sensor);
    temp_sensor_start();
}

float SystemMonitor::getCpuTemperature(){
    float temp = 0;
    temp_sensor_read_celsius(&temp);
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

