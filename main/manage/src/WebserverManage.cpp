#include "WebServerManage.h"
using namespace manager;

WebServerManage::WebServerManage(){
    // 	ap_ip=IPAddress(AP_IP);
// 	ap_gateway=IPAddress(AP_GATEWAY);
// 	ap_subnet=IPAddress(AP_SUBNET);

}
WebServerManage::~WebServerManage(){

}

void WebServerManage::initAP(){
	// server->mode()
	// server->AP();
    //   WiFi.mode(WIFI_AP_STA);  // 先切换到混合模式
//     WiFi.softAPConfig(ap_ip, ap_gateway, ap_subnet);
    
// 	bool ap_started = WiFi.softAP(*ap_ssid, *ap_password);
// #ifdef _DEBUGE_
// 	if (ap_started) {
// 		DEBUGE_PRINTLN("启动到AP模式");
// 		DEBUGE_PRINT("AP名称:");
// 		DEBUGE_PRINTLN(*ap_ssid);
// 		DEBUGE_PRINT("WiFi已连接,访问地址:http://");
// 		DEBUGE_PRINTLN(WiFi.softAPIP());
//   	} else {
// 		DEBUGE_PRINTLN("启动到AP模式失败!");
//   	}
// #endif

}
void WebServerManage::initSTA(){
     	// DEBUGE_PRINTLN("启动到STA模式");
// 	WiFi.begin(sta_ssid.c_str(), sta_password.c_str());

}
void WebServerManage::readHtml(string htmlPath){
    // if(!LittleFS.begin(true)){
// 		DEBUGE_PRINT("LittleFS初始化失败!");
// 	}
// 	File file = LittleFS.open(htmlPath, "r");
// 	if(!file){
// 		DEBUGE_PRINT("LittleFS初始化成功,但无法打开wifi.html文件!");  
// 	}
// 	index_html=LittleFS.open(htmlPath, "r").readString();
// 	file.close();
}
void WebServerManage::loadConfig(){
    prefs->begin("wifi_config");  // 打开名为"wifi_config"的命名空间（只读）
	// staSsid = prefs->read("ssid");
	// sta_password=prefs->read();
// 	prefs->end();
#ifdef _DEBUGE_
	if (staSsid != "") {
		DEBUGE_PRINTF("读取到保存的WiFi: %s\n", sta_ssid.c_str());
	} else {
		DEBUGE_PRINT("未读取到保存的WiFi配置\n");
	}
#endif

}

void WebServerManage::saveWifi(string ssid,string password){
	prefs->begin("wifi_config");  // 打开命名空间（可写）
	prefs->save("ssid", ssid);
	prefs->save("password", password);
// 	prefs->end();
	printf("已保存WiFi: %s\n", ssid.c_str());
// 	DEBUGE_PRINTF("已保存WiFi: %s\n", ssid.c_str());
}
string WebServerManage::getWifiScanOptions(){
    
    string options = "";
  
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
		string ssid = wifiList[i].ssid;
		int rssi = wifiList[i].rssi;
		
		// 过滤空SSID
		// if (ssid.isEmpty()) continue;
		
		// 信号强度显示
		string signal = "";
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
void  WebServerManage::wifiConfigWebServer(){
    // 	AP_init();
// 	read_html("/wifi.html");

	server->on("/",HTTP_GET,[this](WebServerRequest *request){
		this->configRoot(request);
	});
	server->on("/save", HTTP_POST,[this](WebServerRequest *request){
		this->saveConfig(request);
	});
	// server->onNotFound([this](WebServerRequest *request){
	// 	this->notFound(request);
	// });
	server->begin();
	// printf("");
// 	DEBUGE_PRINTLN("WifiConfigServer启动,等待配置...");

}
void WebServerManage::configRoot(WebServerRequest *request){

	// string html = index_html;
	 

	string ssidOptions=getWifiScanOptions();

	//替换占位符
	// html.replace("%SSID_OPTIONS%", ssidOptions);  // 填充WiFi列表
	// html.replace("%STATUS%", "请选择WiFi并输入密码(无密码则留空)");  // 状态提示
	// printf("");
	// 	DEBUGE_PRINTLN(html);
	// request->send(200, "text/html", html);
}
// void WebServers::save_config(AsyncWebServerRequest *request){
// 	if (request->hasArg("ssid") && request->hasArg("password")) {
//     String new_ssid = request->arg("ssid");
//     String new_password = request->arg("password");
    
//     // 保存配置
//     save_wifi(new_ssid, new_password);
    
//     // 更新STA参数并尝试连接
//     sta_ssid = new_ssid;
//     sta_password = new_password;
//     STA_init();

//     // 向网页返回连接状态
//     String html = index_html;
//     html.replace("%STATUS%", "配置已保存，正在连接...请等待重启");
//     request->send(200, "text/html", html);

//     // 延迟2秒后重启ESP32，使配置生效
//     delay(2000);
//     ESP.restart();
//   } else {
//     request->send(400, "text/plain", "参数错误");
//   }

// }
void WebServerManage::indexWebServer(){
    // read_html("/index.html");

	server->on("/",HTTP_GET,[this](WebServerRequest *request){
		this->indexRoot(request);
	});
	// server->on("/reboot",HTTP_POST,[this](WebServerRequest *request){
	// 	// this->reboot(request);
	// });
	server->on("/data",HTTP_GET,[this](WebServerRequest *request){
		this->getSensorData(request);
	});
	server->on("/device-info",HTTP_GET,[this](WebServerRequest*request){
		this->loadDeviceInfo(request);
	});
	// server->on("/update",HTTP_POST,[this](WebServerRequest*request){
	// 	string filename, size_t index, uint8_t *data, size_t len, bool final);
	// 	this->updateSystem(request,filename,index,data,len,final);
	// });
	server->on("/on",HTTP_POST,[this](WebServerRequest*request){
		this->ledOn(request);
	});
	server->on("/off",HTTP_POST,[this](WebServerRequest *request){
		this->ledOff(request);
	});
// 	server->onNotFound([this](AsyncWebServerRequest *request){
// 		this->not_found(request);
// 	});
	server->begin();
	// printf("....");
// 	DEBUGE_PRINTLN("index server启动");

}
void WebServerManage::indexRoot(WebServerRequest *request){
    	// String html=index_html;

// 	DEBUGE_PRINTLN(html);

// 	request->send(200, "text/html", html);
// }
// void WebServers::reboot(AsyncWebServerRequest *request){
// 	DEBUGE_PRINTLN("重启设备\n");
// 	// request->send()
// 	delay(2000);
// 	esp_restart();

}
void WebServerManage::getCPUTemaputer(WebServerRequest *request){

}
void WebServerManage::ledOn(WebServerRequest *request){
    // 	led->on();
	request->send(200, "text/plain", "LED 已打开");
}
void WebServerManage::ledOff(WebServerRequest *request){
     	// led->off();
	request->send(200, "text/plain", "LED 已关闭");
}
void WebServerManage::getSensorData(WebServerRequest *request){
     	// SensorData sensorData;
// 	sensor->GetData(&sensorData);
// 	// cpuMonitor->update();

// 	JsonDocument doc; 

// 	doc["type"] = "sensor_data";
// 	doc["cpuTemp"] =sensorData.cpuTemperature;
// 	doc["cpuUsage"]=50;
// 	// doc["humi"] = sensorData.humidity;

// 	// 转换为JSON字符串并返回
// 	String jsonString;
// 	serializeJson(doc, jsonString);
// 	request->send(200,"application/json",jsonString);
// }
// void WebServers::load_device_info(AsyncWebServerRequest *request){
// 	JsonDocument doc;
// 	doc["type"]="device_info";
// 	doc["version"]="0.0.1";
// 	doc["uptime"]=1;
// 	doc["usedStorage"]=1;
// 	doc["totalStorage"]=3;
// 	doc["rssi"]=20;
// 	doc["ip"]=1922;


// 	String jsonString;
// 	serializeJson(doc,jsonString);
// 	request->send(200,"application/json",jsonString);

}
void WebServerManage::updateSystem(WebServerRequest*reauest,string filename,size_t index,
                                     uint8_t *data, size_t len, bool final){
     	
        // if(index == 0){
// 		// 上传开始：初始化OTA更新
// 		DEBUGE_PRINTF("开始接受固件:%s\n", filename.c_str());
		
// 		// 检查文件是否为.bin格式
// 		if(!filename.endsWith(".bin")){
// 			request->send(400, "text/plain", "错误：请上传.bin格式的固件");
// 			return;
// 		}
		
// 		// 启动OTA更新
// 		if(!Update.begin(UPDATE_SIZE_UNKNOWN)){
// 			Update.printError(Serial);
// 			request->send(500, "text/plain", "OTA初始化失败");
// 			return;
// 		}
// 	}

// 	// 上传中：写入固件数据（包括第一次和中间的数据块）
// 	if (len > 0) {
// 		if (Update.write(data, len) != len) {
// 			Update.printError(Serial);
// 			// 可以选择在这里返回错误，或者继续尝试
// 		}
// 	}

// 	if(final){
// 		// 上传完成：结束OTA更新并重启
// 		if(Update.end(true)){ // true表示更新后重启
// 			DEBUGE_PRINTF("固件更新完成，总大小：%u bytes\n", index + len);
// 			request->send(200, "text/plain", "更新成功，设备即将重启");
// 			delay(1000);
// 			esp_restart();
// 		} else {
// 			Update.printError(Serial);
// 			request->send(500, "text/plain", "固件验证失败");
// 		}
// 	}

}
void WebServerManage::notFound(WebServerRequest *request){
    // / 	DEBUGE_PRINT("未找到处理函数的请求路径:");
// 	DEBUGE_PRINTLN(request->url()); 
// 	DEBUGE_PRINT("请求方法");
// 	DEBUGE_PRINTLN(request->method()==HTTP_GET?"GET":"POST");
// 	request->send(404, "text/plain", "Not found");

}
void WebServerManage::getSystemSetting(WebServerRequest*request){

}

void WebServerManage::begin(){
    // pinMode(2, OUTPUT);
	loadConfig();
	if(staSsid!=""){
		// printf("";)
// 		DEBUGE_PRINTF("尝试连接已保存的WiFi: %s\n", sta_ssid.c_str());
// 		STAInit();
// 		unsigned long start = millis();
// 		while (WiFi.status() != WL_CONNECTED && millis() - start < 10000) {
// 			delay(500);
// 			DEBUGE_PRINT(".");
// 		}
// 		if (WiFi.status() == WL_CONNECTED) {
// 			DEBUGE_PRINTLN("\nSTA连接成功!");
// 			DEBUGE_PRINT("WiFi已连接,访问地址:http://");
// 			DEBUGE_PRINTLN(WiFi.localIP());
// 			index_web_server();
// 			return;  // 跳过后续AP配置步骤
// 		} else {
// 			DEBUGE_PRINTLN("\nSTA连接失败,启动配置模式...");
// 			wifi_config_server();
// 		}
	}
	wifiConfigWebServer();

}
void WebServerManage::setup(){

}
