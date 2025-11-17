// #include <stdio.h>
#include "fileSystem.hpp"
// #include "WebServerManage.h"
// #include "freertos/FreeRTOS.h"
// #include "WebServerManage.h"
// extern "C" void app_main(void){

// }

#include <stdio.h>
#include <Webserver.h>
using namespace core;
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_task_wdt.h"
#include <stdio.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_log.h"

#include "JsonDocument.hpp"
static constexpr const char* TAG = "Main";  // 在类内定
string test_html;
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
	FileSystem fileSystem;
	string word=R"(
<!DOCTYPE html> 
<html>

<head>
	<title>ESP32 联网配置界面</title>
	<meta charset="UTF-8">
	<meta name="viewport" content="width=device-width, initial-scale=1"> 
	<style>
		body {
			font-family: Arial;
			text-align: center;
			margin: 0;
			padding: 0px;
			background-color: aliceblue;
		}

		.wifi-form {
			width: 80%;
			max-width: 500px;
			margin: 50px auto;
			padding: 20px;
			border-radius: 10px;
			box-shadow: 0 0 10px rgba(0, 0, 0, 0.2);
			background-color: rgb(255, 252, 252);
		}

		.wifi-select {
			display: block;
			width: 80%;
			margin: 15px auto;
			padding: 8px;
			font-size: 16px;
		}

		.wifi-input {
			display: block;
			width: 76%;
			margin: 15px auto;
			padding: 8px;
			font-size: 16px;
		}

		.submit-btn {
			display: block;
			margin: 20px auto;
			padding: 10px 20px;
			font-size: 16px;
			background-color: #4CAF50;
			color: white;
			border: none;
			border-radius: 5px;
			cursor: pointer;
		}
	</style>
</head>

<body>
	<h1 style="text-align: center;">配置WiFi连接</h1>
	<form class="wifi-form" method="POST" action="/save">
		<label style="display: block; text-align: center;">选择WiFi网络 (SSID):</label>
		<select class="wifi-select" name="ssid" required>
			<option value="">-- 请选择WiFi --</option>
			%SSID_OPTIONS%
		</select>

		<label style="display: block; text-align: center;">WiFi密码:</label>
		<input class="wifi-input" type="password" name="password" placeholder="无密码则留空">

		<input class="submit-btn" type="submit" value="保存并连接">

	</form>
	<p id="status" style="text-align: center;">%STATUS%</p> 
</body>
</html>)";



	string fileName="wifii.html";
	fileSystem.write(&fileName,&word);

	ESP_LOGI(TAG,"HTML%s",test_html.c_str());
    WebServer server;

	const string Ssid="TPLK";
	const string Password="Wang5203714";

    server.STA(Ssid,Password);

   	server.begin();

	test_html=fileSystem.read(&fileName);
	// ESP_LOGI("HTML","JSON:%s",test_html.c_str());

    server.on("/", HTTP_GET, [](WebServerRequest* req){
		req->send(200, "text/html",test_html.c_str());
	});


//  example_usage();


    while(1){
        static int counter = 0;
        if (++counter % 20 == 0) {  // 每10秒打印一次
            ESP_LOGI("Main", "System running - Free heap: %lu bytes", esp_get_free_heap_size());
        }
        
        vTaskDelay(500 / portTICK_PERIOD_MS);  // 延迟500毫秒

    }
        
}