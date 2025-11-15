#pragma once

#include "nvs_flash.h"
#include "nvs.h"
#include <string>
using namespace std;
namespace core{
    class Preference{
        private:
            nvs_handle_t nvsHandle;
            esp_err_t espError;


        private:
            void initNVS();
        public:
            void begin(string nameSpace);

            template<typename T>
            void save(const string&dataName,const T&data);
            template<typename T> 
            T read(const string &key);
            


    };
}