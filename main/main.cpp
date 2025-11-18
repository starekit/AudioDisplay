// #include <stdio.h>
// #include "WebServerManage.h"
// #include "freertos/FreeRTOS.h"
// #include "WebServerManage.h"
// extern "C" void app_main(void){

// }

#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_task_wdt.h"
#include <string.h>
#include "esp_system.h"
#include "esp_log.h"

#include <Webserver.h>
#include "JsonDocument.hpp"
#include "fileSystem.hpp"

#define _DEBUGE_
#ifdef _DEBUGE_
#include "update.h"
using namespace debuge;

#endif
// #include "bluetooth.h"
using namespace core;
void example_usage() {
    // 创建文档
    JsonDocument doc;
    
    // 使用类似 ArduinoJson 的语法
    doc["type"] = "sensor_data";
    doc["temperature"] = 25.6;
    doc["humidity"] = 65.2;
    doc["timestamp"] = 1234567890;
    doc["is_valid"] = true;
    
    // 读取值
    std::string type = doc["type"];
    double temp = doc["temperature"];
    bool valid = doc["is_valid"];
    
    ESP_LOGI("JSON", "Type: %s, Temp: %.1f, Valid: %d", 
             type.c_str(), temp, valid);
    
    // 序列化
    std::string json_str = doc.serialize();
    ESP_LOGI("JSON", "Serialized: %s", json_str.c_str());
    
    // 嵌套对象示例
    JsonDocument nested_doc;
    nested_doc["sensor"] = "DHT22";
    nested_doc["location"] = "room_1";
    
    // 注意：这里需要扩展 JsonDocument 类来支持嵌套对象
    // 在实际实现中，可以添加 addObject 方法
};

extern "C" void app_main(void){
#ifdef _DEBUGE_
	Update update;
#endif


//  example_usage();


    while(1){
        static int counter = 0;
        if (++counter % 20 == 0) {  // 每10秒打印一次
            ESP_LOGI("Main", "System running - Free heap: %lu bytes", esp_get_free_heap_size());
        }
        
        vTaskDelay(500 / portTICK_PERIOD_MS);  // 延迟500毫秒

    }
        
}