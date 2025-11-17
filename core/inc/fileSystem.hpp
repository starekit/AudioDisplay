#ifndef FILE_SYSTEM_HPP_
#define FILE_SYSTEM_HPP_
#include "string"
#include "esp_littlefs.h"
#include <sys/stat.h>
#include "esp_log.h"
#include <memory>
#define _ESP_DEBUGE_
using namespace std;
namespace core{
	class FileSystem{
		private:
		#ifdef _ESP_DEBUGE_
		  	static constexpr const char* TAG = "FileSystem";  // 在类内定义 TAG
		#endif
			const string path="/data";

			esp_vfs_littlefs_conf_t conf{
				.base_path="/data",
				.partition_label="data",
				.format_if_mount_failed=true,
				.dont_mount=false
			};
			
			uint64_t maxFileHeapSize=1000000;
        public:
			FileSystem();
			~FileSystem();

        private:
			void initLittlefs();

	        bool writeHTML();
        	bool writeTXT();
        	bool writeLOG();

        public:
			void createDirectory(const string dicPath); 
			string read(const string* fileName);
			void write(const  string *fileName,const string *file);
			void del(const string *fileName);
			void list();
		



	};

	FileSystem::FileSystem(){
		mkdir(path.c_str(),0755);
		initLittlefs();
	}
	void FileSystem::createDirectory(const string dicPath){
		mkdir(("/"+dicPath).c_str(),0755);
	}
	void FileSystem::initLittlefs(){
		esp_err_t err=esp_vfs_littlefs_register(&conf);
		if(err!=ESP_OK){
		
			if (err == ESP_FAIL) {
				ESP_LOGE(TAG, "Failed to mount or format filesystem");
			} else if (err == ESP_ERR_NOT_FOUND) {
				ESP_LOGE(TAG, "Failed to find LittleFS partition");
			} else {
				ESP_LOGE(TAG, "Failed to initialize LittleFS (%s)");
			}
			return;
		}
		 size_t total = 0, used = 0;
		err = esp_littlefs_info(conf.partition_label, &total, &used);
		if (err != ESP_OK) { 
			// ESP_LOGE(TAG, "Failed to get LittleFS partition information (%s)", esp_err_to_name(ret));
		} else {
			ESP_LOGI(TAG, "Partition size: total: %d, used: %d", total, used);
		}
				

	}

	string FileSystem::read(const string* fileName){

		auto f = fopen((path+"/"+*fileName).c_str(), "rb");
		
		if (f == NULL) {
			ESP_LOGE(TAG, "Failed to open file for reading");
			return "EEEOR";
		}
		// 检查文件大小
		size_t buffer_size = 10000;
    	char* buffer = (char*)malloc(buffer_size);
    	if (!buffer) ESP_LOGE(TAG,"Faild malloc memory!");
    
		size_t len = 0;
		int c;
		
		while ((c = fgetc(f)) != EOF) {
			// 需要更多空间
			if (len + 1 >= buffer_size) {
				buffer_size *= 2;
				char *new_buffer = (char*)realloc(buffer, buffer_size);
				if (!new_buffer) {
					free(buffer);
					ESP_LOGE(TAG,"Faild realloc memory!");
					return "ERROR";
				}
				buffer = new_buffer;
			}
			
			buffer[len++] = c;
		}
		if (len == 0 && c == EOF) {
			free(buffer);
			return "ERROR";  // 文件结束
		}
		
		buffer[len] = '\0';
		string out(buffer);
		free(buffer);

		// if (!out.empty()) {
		// // 检查是否包含异常字符
		// 	for (char ch : out) {
		// 		if (static_cast<unsigned char>(ch) > 127) {
		// 			ESP_LOGI(TAG, "发现非ASCII字符,可能编码不匹配");
		// 			break;
		// 		}
		// 	}
		// }
	    return out;
    }
	void FileSystem::write(const string*fileName ,const string *file){
		
		if (fileName == nullptr || file == nullptr) {
			ESP_LOGE("FileSystem", "Null pointer passed to write function");
			return;
    	}
		auto *f =fopen((path+"/"+*fileName).c_str(),"wb");

		if(f==nullptr){
			ESP_LOGE(TAG,"Failed to open file for writing: %s", (path+"/"+*fileName).c_str());
			return;
		}
		// ESP_LOGI("Write:","data:%s",file->c_str());
		fwrite(file->c_str(), sizeof(char), file->size(), f);
		fflush(f);
		fclose(f);

	}
	void FileSystem::del(const string*fileName){
		// unlink("/littlefs/greeting.txt");
	}
	void FileSystem::list(){
		   // 列出目录内容
		// ESP_LOGI(TAG, "Listing directory:");
		// DIR* dir = opendir("/littlefs");
		// if (dir) {
		// 	struct dirent* entry;
		// 	while ((entry = readdir(dir)) != NULL) {
		// 	ESP_LOGI(TAG, "Found: %s", entry->d_name);
		// 	}
		// 	closedir(dir);
		// }
	}
}

#endif