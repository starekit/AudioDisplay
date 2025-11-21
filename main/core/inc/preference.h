#pragma once
#include <variant>
#include "core.hpp"
namespace core{
    #define MAX_READ_CHAR_BUFFER_LEN 64
    // #define FILE_ERR  0

    class Preference{
        private:

            static constexpr const char *TAG="Preference";
            nvs_handle_t nvsHandle;
            esp_err_t espError;
            size_t max_read_char_buffer_=MAX_READ_CHAR_BUFFER_LEN;
            void end();
        public:
            esp_err_t open(std::string nameSpace);
            Preference(){
            };
            ~Preference(){
            }

                        /**
             * 删除命名空间中的所有键（模拟删除命名空间）
             * @param namespace_name 要清空的命名空间名称
             * @return true 成功, false 失败
             */
            bool eraseNamespace(const std::string& namespace_name) {
                nvs_handle_t handle;
                esp_err_t err;
                
                // 以读写模式打开命名空间
                err = nvs_open(namespace_name.c_str(), NVS_READWRITE, &handle);
                if (err != ESP_OK) {
                    ESP_LOGE(TAG, "无法打开命名空间 '%s': %s", 
                            namespace_name.c_str(), esp_err_to_name(err));
                    return false;
                }
                
                // 删除命名空间中的所有条目
                err = nvs_erase_all(handle);
                if (err != ESP_OK) {
                    ESP_LOGE(TAG, "删除命名空间 '%s' 失败: %s", 
                            namespace_name.c_str(), esp_err_to_name(err));
                    nvs_close(handle);
                    return false;
                }
                
                // 提交更改
                err = nvs_commit(handle);
                if (err != ESP_OK) {
                    ESP_LOGE(TAG, "提交更改失败: %s", esp_err_to_name(err));
                    nvs_close(handle);
                    return false;
                }
                
                nvs_close(handle);
                ESP_LOGI(TAG, "命名空间 '%s' 已清空", namespace_name.c_str());
                return true;
            }

            /**
             * 完全擦除整个 NVS 分区（删除所有命名空间）
             * 警告：这会删除所有 NVS 数据！
             */
            bool erase_entire_nvs_partition() {
                ESP_LOGI(TAG, "开始擦除整个 NVS 分区...");
                
                // 先反初始化 NVS
                nvs_flash_deinit();
                
                // 擦除整个 NVS 分区
                esp_err_t err = nvs_flash_erase();
                if (err != ESP_OK) {
                    ESP_LOGE(TAG, "擦除 NVS 分区失败: %s", esp_err_to_name(err));
                    return false;
                }
                
                // 重新初始化 NVS
                err = nvs_flash_init();
                if (err != ESP_OK) {
                    ESP_LOGE(TAG, "重新初始化 NVS 失败: %s", esp_err_to_name(err));
                    return false;
                }
                
                ESP_LOGI(TAG, "NVS 分区已完全擦除并重新初始化");
                return true;
            }
            template<typename T>
            void save(std::string nameSpace,const std::string &key,const T&data);
            template<typename T> 
            T read(std::string nameSpace,const std::string &key);


    };
    template<typename T>
    void Preference::save(std::string nameSpace,const std::string &key, const T &data){
        esp_err_t err = nvs_open(nameSpace.c_str(), NVS_READWRITE, &nvsHandle);
        if (err != ESP_OK) {
            ESP_LOGE(TAG, "打开NVS失败: %s", esp_err_to_name(err));
            return;
        }

        esp_err_t set_err = ESP_OK;
        if constexpr (std::is_same_v<T,int8_t>){            
            set_err = nvs_set_i8(nvsHandle, key.c_str(), data);
        }
        else if constexpr(std::is_same_v<T,int16_t>){            
            set_err = nvs_set_i16(nvsHandle, key.c_str(), data);
        }
        else if constexpr(std::is_same_v<T,int32_t>){            
            set_err = nvs_set_i32(nvsHandle, key.c_str(), data);
        }
        else if constexpr(std::is_same_v<T,int64_t>){            
            set_err = nvs_set_i64(nvsHandle, key.c_str(), data);
        }
        else if constexpr (std::is_same_v<T,const char *>){            
            set_err = nvs_set_str(nvsHandle, key.c_str(), data);
        }
        else if constexpr (std::is_same_v<T,std::string>){            
            set_err = nvs_set_str(nvsHandle, key.c_str(), data.c_str());
        }
        else if constexpr (std::is_same_v<T,uint8_t>){            
            set_err = nvs_set_u8(nvsHandle, key.c_str(), data);
        }
        else if constexpr (std::is_same_v<T,uint16_t>){            
            set_err = nvs_set_u16(nvsHandle, key.c_str(), data);
        }
        else if constexpr(std::is_same_v<T,uint32_t>){            
            set_err = nvs_set_u32(nvsHandle, key.c_str(), data);
        }
        else if constexpr(std::is_same_v<T,uint64_t>){            
            set_err = nvs_set_u64(nvsHandle, key.c_str(), data);
        }
        else{            
            ESP_LOGE(TAG, "SAVE:存储类型错误,未知类型");
            set_err = ESP_FAIL;
        }

        if (set_err == ESP_OK) {
            err = nvs_commit(nvsHandle);
            if (err != ESP_OK) {
                ESP_LOGE(TAG, "提交NVS更改失败: %s", esp_err_to_name(err));
            }
        } else {
            ESP_LOGE(TAG, "设置NVS值失败: %s", esp_err_to_name(set_err));
        }
        
        end();
    }
    template<typename T> 
    T Preference::read(std::string nameSpace,const std::string &key){
        nvs_open(nameSpace.c_str(),NVS_READONLY,&nvsHandle);
        if constexpr (std::is_same_v<T,int8_t>){
            int8_t data;
            nvs_get_i8(nvsHandle,key.c_str(),&data);
            end();
            return data;
        }
        else if constexpr(std::is_same_v<T,int16_t>){
            int16_t data;
            nvs_get_i16(nvsHandle,key.c_str(),&data);
            end();
            return data;
        }
        else if constexpr(std::is_same_v<T,int32_t>){
            int32_t data;
            nvs_get_i32(nvsHandle,key.c_str(),&data);
            end();
            return data;
        }
        else if constexpr(std::is_same_v<T,int64_t>){
            int64_t data;
            nvs_get_i64(nvsHandle,key.c_str(),&data);
            end();
            return data;
        }
        else if constexpr (std::is_same_v<T,std::string>){            
            size_t len = 0;
            // 先获取字符串长度
            esp_err_t err = nvs_get_str(nvsHandle, key.c_str(), NULL, &len);
            
            // 分配足够的内存（包括null终止符）
            char *loadData = (char*)malloc(len * sizeof(char));
            if (loadData == NULL) {
                ESP_LOGE(TAG, "内存分配失败");
                end();
                return "";
            }
            
            // 再次调用获取字符串内容
            err = nvs_get_str(nvsHandle, key.c_str(), loadData, &len);
            
            std::string outData;
            if (err == ESP_OK) {
                outData = loadData;
            } else {
                ESP_LOGE(TAG, "读取字符串失败: %s", esp_err_to_name(err));
                outData = "";
            }
            
            free(loadData);
            end();
            return outData;
        }
        else if constexpr (std::is_same_v<T,uint8_t>){
            uint8_t data;
            nvs_get_u8(nvsHandle,key.c_str(),&data);
            end();
            return data;
        }
        else if constexpr (std::is_same_v<T,uint16_t>){
            uint16_t data;
            nvs_get_u16(nvsHandle,key.c_str(),&data);
            end();
            return data;
        }
        else if constexpr(std::is_same_v<T,uint32_t>){
            uint32_t data;
            nvs_get_u32(nvsHandle,key.c_str(),&data);
            end();
            return data;
        }
        else if constexpr(std::is_same_v<T,uint64_t>){
            uint64_t data;
            nvs_get_u64(nvsHandle,key.c_str(),&data);
            end();
            return data;
        }
     
        ESP_LOGE(TAG,"READ:存储类型错误,未知类型");
    
    }

}