#include "audio_system.h"

void System::begin(){
	selectWebserver();
}
void System::selectWebserver(){
	
	if(pref_ptr_->open(NVS_WIFI_NAMESPACE)==ESP_ERR_NVS_NOT_FOUND){
		ESP_LOGE(TAG,"Namesapce not append.Wifi config Webserver Start.");
		webserver_ptr_=WebserverInterface::createWifiserver(pref_ptr_,file_ptr_);
		return;
	}
	ESP_LOGI(TAG,"Have wifi namespace");
	webserver_ptr_=WebserverInterface::createIndexserver(pref_ptr_,file_ptr_,monitor_ptr_);
}