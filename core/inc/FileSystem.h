#pragma once
#include "core.hpp"
#include "esp_littlefs.h"
// #define _WRITEHTML_
#define _ESP_DEBUGE_
#ifdef _WRITEHTML_
#include "html.h"
using namespace html;
#endif


namespace core{
	class FileSystem{
		private:
			#ifdef _WRITEHTML_
				const string wifiWebName="wifi.html";
				const string indexWebName="index.html";
			#endif
		#ifdef _ESP_DEBUGE_
		  	static constexpr const char* TAG = "FileSystem"; 
		#endif
			
			const std::string path="/data";

			esp_vfs_littlefs_conf_t conf{
				.base_path="/data",
				.partition_label="data",
				.format_if_mount_failed=false,
				.dont_mount=false
			};
			
			uint64_t maxFileHeapSize=1000000;
        public:
			FileSystem();
			~FileSystem(){

			}

        private:
			esp_err_t initLittlefs();
			void creDataDic();
			esp_err_t format();
			esp_err_t remountAndFormat();
			bool folderExists(const std::string* path);
			void list(const char* path, int level);
        	// bool writeTXT();
        	// bool writeLOG();

        public:
			void createDirectory(const std::string dicPath); 
			std::string read(const std::string* fileName);
			void write(const  std::string *fileName,const std::string *file);
			void del(const std::string *fileName);
			void listFileTree();
	};
}

