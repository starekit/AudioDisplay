#include "preference.h"
using namespace core;
void Preference::initNVS(){
    esp_err_t ret=nvs_flash_init();
    if(ret==ESP_ERR_NVS_NO_FREE_PAGES||ret==ESP_ERR_NVS_NEW_VERSION_FOUND){
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret=nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);
}
void Preference::begin(string nameSpace){//data
    espError=nvs_open(nameSpace.c_str(),NVS_READWRITE,&nvsHandle);
    if(espError!=ESP_OK){
        printf("Error opening NVS handle!\n");
        //write log
        return;
    }else{
        //create namespace
    }


}

template<typename T>
void Preference::save(const string&key, const T &data){

    if constexpr (is_same_v<T,int8_t>){
        // espError=nvs_get_i32(nvsHandle,key.c_str(),&data);
        nvs_set_i8(nvsHandle,key.c_str(),&data);
    }
    else if constexpr(is_same_v<T,int16_t>){
        nvs_set_i16(nvsHandle,key.c_str(),&data);
    }
    else if constexpr(is_same_v<T,int32_t>){
        nvs_set_i32(nvsHandle,key.c_str(),&data);
    }
    else if constexpr(is_same_v<T,int64_t>){
        nvs_set_i64(nvsHandle,key.c_str(),&data);
    }
    else if constexpr (is_same_v<T,string>){
        nvs_set_str(nvsHandle,key.c_str(),&data);
    }
    else if constexpr (is_same_v<T,uint8_t>){
        nvs_set_u8(nvsHandle,key.c_str(),&data);
    }
    else if constexpr (is_same_v<T,uint16_t>){
        nvs_set_u16(nvsHandle,key.c_str(),&data);
    }
    else if constexpr(is_same_v<T,uint32_t>){
        nvs_set_u16(nvsHandle,key.c_str(),&data);
    }
    else if constexpr(is_same_v<T,uint64_t>){
        nvs_set_u64(nvsHandle,key.c_str(),&data);
    }
    else{
        // printf("存储类型错误,未知类型:\n",T)
    }
    nvs_commit(nvsHandle);

}

template<typename T> 
T Preference::read(const string &key){
    T data;
    if constexpr (is_same_v<T,int8_t>){
        // espError=nvs_get_i32(nvsHandle,key.c_str(),&data);
        nvs_get_i8(nvsHandle,key.c_str(),&data);
    }
    else if constexpr(is_same_v<T,int16_t>){
        nvs_get_i16(nvsHandle,key.c_str(),&data);
    }
    else if constexpr(is_same_v<T,int32_t>){
        nvs_get_i32(nvsHandle,key.c_str(),&data);
    }
    else if constexpr(is_same_v<T,int64_t>){
        nvs_get_i64(nvsHandle,key.c_str(),&data);
    }
    else if constexpr (is_same_v<T,string>){
        nvs_get_str(nvsHandle,key.c_str(),&data);
    }
    else if constexpr (is_same_v<T,uint8_t>){
        nvs_get_u8(nvsHandle,key.c_str(),&data);
    }
    else if constexpr (is_same_v<T,uint16_t>){
        nvs_get_u16(nvsHandle,key.c_str(),&data);
    }
    else if constexpr(is_same_v<T,uint32_t>){
        nvs_get_u16(nvsHandle,key.c_str(),&data);
    }
    else if constexpr(is_same_v<T,uint64_t>){
        nvs_get_u64(nvsHandle,key.c_str(),&data);
    }
    else{
        // printf("存储类型错误,未知类型:\n",T);
        return;
    }
    return data;
}