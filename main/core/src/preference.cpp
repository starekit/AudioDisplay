#include "preference.h"
using namespace core;
void Preference::begin(std::string nameSpace){//data
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
void Preference::save(const std::string&key, const T &data){

    if constexpr (std::is_same_v<T,int8_t>){
        // espError=nvs_get_i32(nvsHandle,key.c_str(),&data);
        nvs_set_i8(nvsHandle,key.c_str(),&data);
    }
    else if constexpr(std::is_same_v<T,int16_t>){
        nvs_set_i16(nvsHandle,key.c_str(),&data);
    }
    else if constexpr(std::is_same_v<T,int32_t>){
        nvs_set_i32(nvsHandle,key.c_str(),&data);
    }
    else if constexpr(std::is_same_v<T,int64_t>){
        nvs_set_i64(nvsHandle,key.c_str(),&data);
    }
    else if constexpr (std::is_same_v<T,std::string>){
        nvs_set_str(nvsHandle,key.c_str(),&data);
    }
    else if constexpr (std::is_same_v<T,uint8_t>){
        nvs_set_u8(nvsHandle,key.c_str(),&data);
    }
    else if constexpr (std::is_same_v<T,uint16_t>){
        nvs_set_u16(nvsHandle,key.c_str(),&data);
    }
    else if constexpr(std::is_same_v<T,uint32_t>){
        nvs_set_u16(nvsHandle,key.c_str(),&data);
    }
    else if constexpr(std::is_same_v<T,uint64_t>){
        nvs_set_u64(nvsHandle,key.c_str(),&data);
    }
    else{
        // printf("存储类型错误,未知类型:\n",T)
    }
    nvs_commit(nvsHandle);

}

template<typename T> 
T Preference::read(const std::string &key){
    T data;
    if constexpr (std::is_same_v<T,int8_t>){
        // espError=nvs_get_i32(nvsHandle,key.c_str(),&data);
        nvs_get_i8(nvsHandle,key.c_str(),&data);
    }
    else if constexpr(std::is_same_v<T,int16_t>){
        nvs_get_i16(nvsHandle,key.c_str(),&data);
    }
    else if constexpr(std::is_same_v<T,int32_t>){
        nvs_get_i32(nvsHandle,key.c_str(),&data);
    }
    else if constexpr(std::is_same_v<T,int64_t>){
        nvs_get_i64(nvsHandle,key.c_str(),&data);
    }
    else if constexpr (std::is_same_v<T,std::string>){
        nvs_get_str(nvsHandle,key.c_str(),&data);
    }
    else if constexpr (std::is_same_v<T,uint8_t>){
        nvs_get_u8(nvsHandle,key.c_str(),&data);
    }
    else if constexpr (std::is_same_v<T,uint16_t>){
        nvs_get_u16(nvsHandle,key.c_str(),&data);
    }
    else if constexpr(std::is_same_v<T,uint32_t>){
        nvs_get_u16(nvsHandle,key.c_str(),&data);
    }
    else if constexpr(std::is_same_v<T,uint64_t>){
        nvs_get_u64(nvsHandle,key.c_str(),&data);
    }
    else{
        // printf("存储类型错误,未知类型:\n",T);
        return;
    }
    return data;
}
void Preference::end(){
    nvs_close(nvsHandle);
}