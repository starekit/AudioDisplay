#ifndef FILE_SYSTEM_HPP_
#define FILE_SYSTEM_HPP_
#include "string"
#include "esp_littlefs.h"
#include <stdio.h>
#include <sys/unistd.h>
#include <sys/stat.h>
#include "esp_err.h"
#include "esp_log.h"

#include <memory>
using namespace std;
namespace core{
	class FileSystem{
		private:

			const string path="/data";

			esp_vfs_littlefs_conf_t conf{
				.base_path="/data",
				.partition_label="data",
				.format_if_mount_failed=true,
				.dont_mount=false,
			};
			uint8_t maxFileHeapSize=10000;
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
				// ESP_LOGE(TAG, "Failed to mount or format filesystem");
			} else if (err == ESP_ERR_NOT_FOUND) {
				// ESP_LOGE(TAG, "Failed to find LittleFS partition");
			} else {
				// ESP_LOGE(TAG, "Failed to initialize LittleFS (%s)", esp_err_to_name(ret));
			}
			return;
		}
		 size_t total = 0, used = 0;
		err = esp_littlefs_info(conf.partition_label, &total, &used);
		if (err != ESP_OK) { 
			// ESP_LOGE(TAG, "Failed to get LittleFS partition information (%s)", esp_err_to_name(ret));
		} else {
			// ESP_LOGI(TAG, "Partition size: total: %d, used: %d", total, used);
		}
				

	}

	string FileSystem::read(const string* fileName){

		 // 读取文件
		// ESP_LOGI(TAG, "Reading file");
		auto f = fopen((path+"/"+*fileName).c_str(), "r");
		if (f == NULL) {
			// ESP_LOGE(TAG, "Failed to open file for reading");
			return;
		}
		
		// string *line;
		char* line=(char*)malloc(maxFileHeapSize*sizeof(char));
		while (fgets(line, sizeof(line), f)) {
			// 移除换行符
			char* pos = strchr(line, '\n');
			if (pos) {
				*pos = '\0';
			}
			// ESP_LOGI(TAG, "Read from file: '%s'", line);
		}
		string outFile=line;
		free(line);
		fclose(f);
		return outFile;
    	}
	void FileSystem::write(const string*fileName ,const string *file){
		auto *f =fopen((path+"/"+*fileName).c_str(),"w");
		if(f==nullptr){
			// printf("")
			return;
		}
		fprintf(f,file->c_str());
		fclose(f);
        	//write html
        	//write txt
	        //write LOG
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