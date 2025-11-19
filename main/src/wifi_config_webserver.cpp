#include "webserver_interface.h"

class WifiConfigWebserver:public WebserverInterface{
	private:
		const std::string ap_ssid_ = "ESP32";
		const std::string ap_password_="12345678";
		static constexpr const char*TAG="WifiConfigWebServer";

	public:
			
		WifiConfigWebserver(){
			ssid_=ap_ssid_;
			password_=ap_password_;
			readHtml("wifi.html");
			
		}
		~WifiConfigWebserver(){

		}

	

		std::string getWifiScanOptions(){
			
			std::string options = "";

			//扫描WiFi并获取数量
			// int n = WiFi.scanNetworks(false, false);  // 不扫描隐藏网络
			// 	Serial.printf("扫描到 %d 个WiFi网络\n", n);

			// if (n == 0) {
			// 	options = "<option value=''>未扫描到WiFi</option>";
			// 	return options;
			// }
			int n=10;

			//  将扫描结果存入自定义数组（WifiInfo结构体）
			WifiInfo* wifiList = new WifiInfo[n];  // 动态创建数组
			for (int i = 0; i < n; i++) {
			// wifiList[i].ssid = WiFi.SSID(i);     // 存储SSID
			// wifiList[i].rssi = WiFi.RSSI(i);     // 存储信号强度
			}

			for (int i = 0; i < n; i++) {                             //!!!待优化
			for (int j = i + 1; j < n; j++) {
			// 若j的信号比i强，则交换i和j的位置
			if (wifiList[j].rssi > wifiList[i].rssi) {
				WifiInfo temp = wifiList[i];
				wifiList[i] = wifiList[j];
				wifiList[j] = temp;
			}
			}
			}

			// 生成下拉选项（从排序后的数组中读取）
			for (int i = 0; i < n; i++) {
			std::string ssid = wifiList[i].ssid;
			int rssi = wifiList[i].rssi;

			// 过滤空SSID
			// if (ssid.isEmpty()) continue;

			// 信号强度显示
			std::string signal = "";
			if (rssi > -50) signal = "（强）";
			else if (rssi > -70) signal = "（中）";
			else signal = "（弱）";

			// 拼接option标签
			options += "<option value=\"" + ssid + "\">" + ssid + " " + signal + "</option>\n";
			}
			delete[] wifiList;
			// 清除WiFi库的扫描缓存
			// WiFi.scanDelete();

			return options;
			
		}

		void saveWifi(std::string ssid,std::string password){
			prefs->begin("wifi_config");  // 打开命名空间（可写）
			prefs->save("ssid", ssid);
			prefs->save("password", password);
			prefs->end();
			ESP_LOGD(TAG,"已保存WiFi: %s\n", ssid.c_str());
		}
		
		void saveConfig(WebServerRequest *request){
			if (request->hasArg("ssid") && request->hasArg("password")) {
			const  std::string newSsid = request->arg("ssid");
			const  std::string newPassword = request->arg("password");

			// 保存配置
			saveWifi(newSsid, newPassword);

			// 更新STA参数并尝试连接
			ssid_ = newSsid;
			ssid_ = newPassword;
			server->STA(ssid_,password_);

			// 向网页返回连接状态
			std::string html = index_html_;
			// html->replace("%STATUS%", "配置已保存，正在连接...请等待重启");
			request->send(200, "text/html", &html);
			// 延迟2秒后重启ESP32，使配置生效
			// delay(2000);
			esp_restart();
			} 
			else {
			std::string error="参数错误";
			request->send(200,"text/html",&error);
			}

		}
		
		void begin(){

			server->AP(ssid_,password_);
			server->begin();

			server->on("/",HTTP_GET,[this](WebServerRequest *request){
				this->root(request);
			});
			server->on("/save", HTTP_POST,[this](WebServerRequest *request){
				this->saveConfig(request);
			});
			// server->onNotFound([this](WebServerRequest *request){
			// 	this->notFound(request);
			// });
			ESP_LOGI(TAG,"WifiConfigServer启动,等待配置...");

		}
		void root(WebServerRequest *request){
			// string html = index_html;


			std::string ssidOptions=getWifiScanOptions();

			//替换占位符
			// html.replace("%SSID_OPTIONS%", ssidOptions);  // 填充WiFi列表
			// html.replace("%STATUS%", "请选择WiFi并输入密码(无密码则留空)");  // 状态提示
			// printf("");
			// 	DEBUGE_PRINTLN(html);
			// request->send(200, "text/html", html);

		}
		
		void notFound(WebServerRequest *request){
			ESP_LOGE(TAG,"未找到处理函数的请求路径:%s",request);
		}



};
