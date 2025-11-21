#pragma once
#include "core.hpp"
namespace core{
    class Preference{
        private:
            nvs_handle_t nvsHandle;
            esp_err_t espError;
        public:
            Preference(){
            };
            ~Preference(){
            }
            void begin(std::string nameSpace);

            template<typename T>
            void save(const std::string&dataName,const T&data);
            template<typename T> 
            T read(const std::string &key);
            void end();
            


    };
}