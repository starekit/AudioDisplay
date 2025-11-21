#include "webserver_interface.h"

namespace server{
	WebserverInterface *WebserverInterface::webserver_instance_ = nullptr;

	class IndexWebserver:public WebserverInterface{

		private:
			static constexpr const char* TAG="IndexWebServer:";

			static constexpr const char* sta_ssid_="TPLK";
			static constexpr const char* sta_password_="Wang5203714";
	

			std::function<float(void)> used_callback_;
			std::function<float(void)> temperature_callback_;

			std::shared_ptr<SystemMonitor> system_monitor_ptr_;
		private:
			void setUsedCallback(const std::function<float(void)>& cb) {
				used_callback_ = cb;
			}
			void setTemperatureCallback(const std::function<float(void)>& cb){
				temperature_callback_=cb;
			}

		public:
			IndexWebserver(std::shared_ptr<Preference> prefs,std::shared_ptr<FileSystem>file,std::shared_ptr<SystemMonitor> systemMonitor):system_monitor_ptr_(systemMonitor){
			 	initInterface(prefs,file);
				startServer();
				
				readHtml("index.html");
				begin();
			}
			~IndexWebserver(){
				endServer();
			}
			void root(WebServerRequest *request){
				ESP_LOGI(TAG,"%s",index_html_);
				request->sendHtml(index_html_);
			}
			void updateSystem(WebServerRequest *request,std::string filename, size_t index, uint8_t *data, size_t len, bool final){

				// if(filename.substr(filename.rfind('.'))!=".bin"){
				// 	ESP_LOGE(TAG,"文件类型%s",filename.rfind('.'));
				// 	request->send(200, "text/plain", "错误：请上传.bin格式的固件");
				// 	return;
				// }


				if(index == 0){
					// 上传开始：初始化OTA更新
					ESP_LOGI(TAG,"开始接受固件:%s\n", filename.c_str());

					
					// // 启动OTA更新
					// if(!Update.begin(UPDATE_SIZE_UNKNOWN)){
					// 	Update.printError(Serial);
					// 	request->send(500, "text/plain", "OTA初始化失败");
					// 	return;
					// }
				}

				// 上传中：写入固件数据（包括第一次和中间的数据块）
				if (len > 0) {
					ESP_LOGE(TAG,"写入数据中");
					ESP_LOGI(TAG,"Data:%s",data);
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
				doc["name"]=NAME;
				doc["version"]=FIRMWARE_VERSION;
				doc["uptime"]=1;
				doc["usedStorage"]=1;
				doc["totalStorage"]=3;
				doc["rssi"]=-20;
				doc["ip"]="192.168.0.100";

				request->sendJson(200,doc.serialize());

			}
			void getSensorData(WebServerRequest *request){
				JsonDocument doc; 

				doc["type"] = "sensor_data";
				doc["cpuTemp"] =system_monitor_ptr_->getCpuTemperature();
				doc["cpuUsage"]=system_monitor_ptr_->getCpuUsed();
				doc["humi"] = 50;

				request->sendJson(200,doc.serialize());
			}
			void reboot(WebServerRequest *request){
				// esp_rom_delay_us()
				// delay(2000);
				esp_restart();
			}
			void ledOn(WebServerRequest *request){
				// 	led->on();
				request->sendText("LED 已打开");
				ESP_LOGI(TAG,"LED打开");
			}
			void ledOff(WebServerRequest *request){
				// led->off();
				// request->send(200, "text/plain", "LED 已关闭");
				request->sendText("LED 已关闭");
				ESP_LOGI(TAG,"LED关闭");
			}
			void getSystemSetting(WebServerRequest*request){

			}
			void begin(){
				wifi_ptr_->STA(sta_ssid_,sta_password_);

				on("/",HTTP_GET,[this](WebServerRequest *request){
					root(request);
				});
				on("/reboot",HTTP_POST,[this](WebServerRequest *request){
					reboot(request);
				});
				on("/data",HTTP_GET,[this](WebServerRequest *request){
					getSensorData(request);
				});
				on("/device-info",HTTP_GET,[this](WebServerRequest*request){
					loadDeviceInfo(request);
				});
				onUpload("/update",HTTP_POST,[this](WebServerRequest*request, std::string filename, size_t index, uint8_t *data, size_t len, bool final){
					updateSystem(request,filename,index,data,len,final);
				});
				on("/on",HTTP_POST,[this](WebServerRequest*request){
					ledOn(request);
				});
				on("/off",HTTP_POST,[this](WebServerRequest *request){
					ledOff(request);
				});
				// server->onNotFound([this](WebServerRequest *request){
				// 	this->not_found(request);
				// });
				ESP_LOGV(TAG,"index server启动");


			}
			void notFound(WebServerRequest *request){
				ESP_LOGE(TAG,"未找到处理函数的请求路径:%s",request);
			}
	};
	std::unique_ptr<WebserverInterface> WebserverInterface::createIndexserver(std::shared_ptr<Preference> prefs,std::shared_ptr<FileSystem>file,std::shared_ptr<SystemMonitor> systemMonitor){
		return std::make_unique<IndexWebserver>(prefs,file,systemMonitor);
	}
}
