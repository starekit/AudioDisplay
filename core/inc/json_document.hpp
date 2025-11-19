#ifndef JSON_DOCUMN_HPP_
#define JSON_DOCUMN_HPP_ 
#include "core.hpp"
#include "cJSON.h"
class JsonDocument{
    private:
    cJSON* root_;
    bool should_cleanup_;


    public:
    // 构造函数
    JsonDocument() : root_(cJSON_CreateObject()), should_cleanup_(true) {}
    
    JsonDocument(const char* json_str) : should_cleanup_(true) {
        root_ = cJSON_Parse(json_str);
        if (!root_) {
            root_ = cJSON_CreateObject();
        }
    }
    
    // 析构函数
    ~JsonDocument() {
        if (should_cleanup_ && root_) {
            cJSON_Delete(root_);
        }
    }

    class JsonValueProxy {
    private:
        cJSON* parent_;
        const char* key_;
        
    public:
        JsonValueProxy(cJSON* parent, const char* key) : parent_(parent), key_(key) {}
        
        // 赋值操作符重载
        JsonValueProxy& operator=(const char* value) {
            if (parent_ && key_) {
                cJSON* item = cJSON_GetObjectItem(parent_, key_);
                if (item) {
                    if (cJSON_IsString(item)) {
                        cJSON_SetValuestring(item, value);
                    } else {
                        cJSON_DeleteItemFromObject(parent_, key_);
                        cJSON_AddStringToObject(parent_, key_, value);
                    }
                } else {
                    cJSON_AddStringToObject(parent_, key_, value);
                }
            }
            return *this;
        }
        
        JsonValueProxy& operator=(int value) {
            if (parent_ && key_) {
                cJSON* item = cJSON_GetObjectItem(parent_, key_);
                if (item) {
                    if (cJSON_IsNumber(item)) {
                        cJSON_SetNumberValue(item, value);
                    } else {
                        cJSON_DeleteItemFromObject(parent_, key_);
                        cJSON_AddNumberToObject(parent_, key_, value);
                    }
                } else {
                    cJSON_AddNumberToObject(parent_, key_, value);
                }
            }
            return *this;
        }
        
        JsonValueProxy& operator=(double value) {
            if (parent_ && key_) {
                cJSON* item = cJSON_GetObjectItem(parent_, key_);
                if (item) {
                    if (cJSON_IsNumber(item)) {
                        cJSON_SetNumberValue(item, value);
                    } else {
                        cJSON_DeleteItemFromObject(parent_, key_);
                        cJSON_AddNumberToObject(parent_, key_, value);
                    }
                } else {
                    cJSON_AddNumberToObject(parent_, key_, value);
                }
            }
            return *this;
        }
        
        JsonValueProxy& operator=(bool value) {
            if (parent_ && key_) {
                cJSON* item = cJSON_GetObjectItem(parent_, key_);
                if (item) {
                    if (cJSON_IsBool(item)) {
                        cJSON_SetBoolValue(item, value);
                    } else {
                        cJSON_DeleteItemFromObject(parent_, key_);
                        cJSON_AddBoolToObject(parent_, key_, value);
                    }
                } else {
                    cJSON_AddBoolToObject(parent_, key_, value);
                }
            }
            return *this;
        }
        
        // 类型转换操作符，用于读取值
        operator std::string() const {
            if (parent_ && key_) {
                cJSON* item = cJSON_GetObjectItem(parent_, key_);
                if (cJSON_IsString(item) && item->valuestring) {
                    return std::string(item->valuestring);
                }
            }
            return "";
        }
        
        operator int() const {
            if (parent_ && key_) {
                cJSON* item = cJSON_GetObjectItem(parent_, key_);
                if (cJSON_IsNumber(item)) {
                    return item->valueint;
                }
            }
            return 0;
        }
        
        operator double() const {
            if (parent_ && key_) {
                cJSON* item = cJSON_GetObjectItem(parent_, key_);
                if (cJSON_IsNumber(item)) {
                    return item->valuedouble;
                }
            }
            return 0.0;
        }
        
        operator bool() const {
            if (parent_ && key_) {
                cJSON* item = cJSON_GetObjectItem(parent_, key_);
                if (cJSON_IsBool(item)) {
                    return cJSON_IsTrue(item);
                }
            }
            return false;
        }
};
    // 重载 [] 操作符
    JsonValueProxy operator[](const char* key) {
        return JsonValueProxy(root_, key);
    }
    
    // 序列化为字符串
    std::string serialize(bool formatted = true) const {
        if (!root_) return "";
        
        char* json_str = formatted ? cJSON_Print(root_) : cJSON_PrintUnformatted(root_);
        if (!json_str) return "";
        
        std::string result(json_str);
        free(json_str);
        return result;
    }
    
    // 从字符串解析
    bool deserialize(const char* json_str) {
        cJSON* new_root = cJSON_Parse(json_str);
        if (!new_root) return false;
        
        if (should_cleanup_ && root_) {
            cJSON_Delete(root_);
        }
        root_ = new_root;
        should_cleanup_ = true;
        return true;
    }
    
    // 检查键是否存在
    bool contains(const char* key) const {
        return root_ && key && cJSON_GetObjectItem(root_, key);
    }
    
    // 获取底层 cJSON 对象（用于高级操作）
    cJSON* cjson() { return root_; }
    const cJSON* cjson() const { return root_; }


};

#endif