#include "preference.h"
using namespace core;
esp_err_t Preference::open(std::string nameSpace){
    return nvs_open(nameSpace.c_str(),NVS_READONLY,&nvsHandle);
}

void Preference::end(){
    nvs_close(nvsHandle);
}

