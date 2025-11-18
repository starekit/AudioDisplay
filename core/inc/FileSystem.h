#pragma once
#include "string"
#include <dirent.h>
#include "esp_littlefs.h"
#include <sys/stat.h>
#include "esp_log.h"
#include <memory>
#include <stdio.h>

// #define _WRITEHTML_
#define _ESP_DEBUGE_
#ifdef _WRITEHTML_
#include "html.h"
using namespace html;
#endif


using namespace std;
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
			const string path="/data";

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
			void initLittlefs();
			void creDataDic();
			esp_err_t format();
			void remountAndFormat();
			bool folderExists(const string* path);
			void list(const char* path, int level);
        	// bool writeTXT();
        	// bool writeLOG();

        public:
			void createDirectory(const string dicPath); 
			string read(const string* fileName);
			void write(const  string *fileName,const string *file);
			void del(const string *fileName);
			void listFileTree();
	};
}

