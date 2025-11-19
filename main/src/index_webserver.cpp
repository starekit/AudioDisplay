#include "webserver_interface.h"

class IndexWebserver:public WebserverInterface{

	private:
		static constexpr const char* TAG="IndexWebServer:";

		std::function<float(void)> used_callback_;
		std::function<float(void)> temperature_callback_;
	private:
		void setUsedCallback(const std::function<float(void)>& cb) {
			used_callback_ = cb;
		}
		void setTemperatureCallback(const std::function<float(void)>& cb){
			temperature_callback_=cb;
		}

	public:
		IndexWebserver(){
			readHtml("index.html");
		}
		~IndexWebserver(){

		}
		void root(WebServerRequest *request){
			std::string html=index_html_;
			ESP_LOGV(TAG,"%s",html);
			request->sendHtml(&html);
		}
		void updateSystem(WebServerRequest *request,std::string filename, size_t index, uint8_t *data, size_t len, bool final){

			if(index == 0){
			// 上传开始：初始化OTA更新
			ESP_LOGI(TAG,"开始接受固件:%s\n", filename.c_str());

			// 检查文件是否为.bin格式
			// if(!filename.endsWith(".bin")){
			// 	request->send(400, "text/plain", "错误：请上传.bin格式的固件");
			// 	return;
			// }

			// 启动OTA更新
			// 		if(!Update.begin(UPDATE_SIZE_UNKNOWN)){
			// 			Update.printError(Serial);
			// 			request->send(500, "text/plain", "OTA初始化失败");
			// 			return;
			// 		}
			}

			// 	// 上传中：写入固件数据（包括第一次和中间的数据块）
			if (len > 0) {
			ESP_LOGE(TAG,"写入数据中");
			// if (Update.write(data, len) != len) {
			// 	Update.printError(Serial);
			// 	// 可以选择在这里返回错误，或者继续尝试
			// }
			}

			if(final){
			ESP_LOGI(TAG,"上传完成");
			// 上传完成：结束OTA更新并重启
			// if(Update.end(true)){ // true表示更新后重启
			// 	DEBUGE_PRINTF("固件更新完成，总大小：%u bytes\n", index + len);
			// 	request->send(200, "text/plain", "更新成功，设备即将重启");
			// 	delay(1000);
			// 	esp_restart();
			// } else {
			// 	Update.printError(Serial);
			// 	request->send(500, "text/plain", "固件验证失败");
			// }
			}

		}
		void loadDeviceInfo(WebServerRequest *request){
			JsonDocument doc;
			doc["type"]="device_info";
			doc["version"]="0.0.1";
			doc["uptime"]=1;
			doc["usedStorage"]=1;
			doc["totalStorage"]=3;
			doc["rssi"]=20;
			doc["ip"]=1922;

			std::string jsonString=doc.serialize();
			request->sendJson(200,&jsonString);

		}
		void getSensorData(WebServerRequest *request){

			// SensorData sensorData;
			// 	sensor->GetData(&sensorData);
			// 	// cpuMonitor->update();

			JsonDocument doc; 

			doc["type"] = "sensor_data";
			// doc["cpuTemp"] =systemMonitor->getCpuTemperature();
			// doc["cpuUsage"]=systemMonitor->getCpuUsed();
			// 	// doc["humi"] = sensorData.humidity;

			// 转换为JSON字符串并返回
			// 序列化
			std::string jsonString = doc.serialize();
			request->sendJson(200,&jsonString);
		}
		void reboot(WebServerRequest *request){
			// esp_rom_delay_us()
			// delay(2000);
			esp_restart();
		}
		void ledOn(WebServerRequest *request){
			// 	led->on();
			// request->sendText(&"LED 已打开");
		}
		void ledOff(WebServerRequest *request){
			// led->off();
			// request->send(200, "text/plain", "LED 已关闭");
		}
		void getSystemSetting(WebServerRequest*request){

		}
		void begin(){
			server->STA(ssid_,password_);
			server->begin();

			server->on("/",HTTP_GET,[this](WebServerRequest *request){
			this->root(request);
			});
			server->on("/reboot",HTTP_POST,[this](WebServerRequest *request){
			this->reboot(request);
			});
			server->on("/data",HTTP_GET,[this](WebServerRequest *request){
			this->getSensorData(request);
			});
			server->on("/device-info",HTTP_GET,[this](WebServerRequest*request){
			this->loadDeviceInfo(request);
			});
			server->onUpload("/update",HTTP_POST,[this](WebServerRequest*request, std::string filename, size_t index, uint8_t *data, size_t len, bool final){
			this->updateSystem(request,filename,index,data,len,final);
			});
			server->on("/on",HTTP_POST,[this](WebServerRequest*request){
			this->ledOn(request);
			});
			server->on("/off",HTTP_POST,[this](WebServerRequest *request){
			this->ledOff(request);
			});
			// server->onNotFound([this](WebServerRequest *request){
			// 	this->not_found(request);
			// });
			ESP_LOGV(TAG,"index server启动");


		}


};
