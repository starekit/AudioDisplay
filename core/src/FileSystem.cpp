#include "FileSystem.h"
using namespace core;

FileSystem::FileSystem(){
    creDataDic();
    initLittlefs();
    #ifdef _WRITEHTML_
        remountAndFormat();
        write(&wifiWebName,&wifiConfigHtml);
        write(&indexWebName,&indexHtml);
    #endif
}

void FileSystem::listFileTree(){
    list("/data",0);
}

esp_err_t FileSystem::format(){
    ESP_LOGI(TAG, "开始格式化 LittleFS...");

    // 方法1: 使用 littlefs_format 函数
    esp_err_t ret =esp_littlefs_format(conf.partition_label);
    if (ret!= ESP_OK) {
        ESP_LOGE(TAG, "格式化失败: %s", esp_err_to_name(ret));
        return ret;
    }
    ESP_LOGI(TAG, "格式化完成");
    return ESP_OK;

}

void FileSystem::remountAndFormat(){
     // 先卸载
    esp_err_t ret = esp_vfs_littlefs_unregister(conf.partition_label);
    if (ret != ESP_OK) {
        ESP_LOGW(TAG, "卸载失败: %s", esp_err_to_name(ret));
    }
    
    // 格式化
    ret = format();
    if (ret != ESP_OK) {

    }
    
    ret = esp_vfs_littlefs_register(&conf);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "重新挂载失败: %s", esp_err_to_name(ret));
    }
    
    ESP_LOGI(TAG, "重新挂载完成");

}

bool FileSystem::folderExists(const string* path){
    struct stat st;
    if (stat(path->c_str(), &st) == 0) {
        return S_ISDIR(st.st_mode);  // 判断是否为目录
    }
    return false;
}

void FileSystem::creDataDic(){
    if (!folderExists(&this->path)) {
        mkdir(this->path.c_str(),0755);
    }
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
    //     for (char ch : out) {
    //         if (static_cast<unsigned char>(ch) > 127) {
    //             ESP_LOGI(TAG, "发现非ASCII字符,可能编码不匹配");
    //             break;
    //         }
    //     }
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

void FileSystem::list(const char* path, int level){
    DIR* dir = opendir(path);
    if (dir == NULL) return;

    struct dirent* entry;
    while ((entry = readdir(dir)) != NULL) {
        // 跳过当前目录和上级目录
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            continue;
        }

        char full_path[1024];
        snprintf(full_path, sizeof(full_path), "%s/%s", path, entry->d_name);
        
        struct stat st;
        if (stat(full_path, &st) == 0) {
            // 缩进显示层级
            for (int i = 0; i < level; i++) {
                printf("  ");
            }
            
            if (S_ISDIR(st.st_mode)) {
                printf("📁 %s/\n", entry->d_name);
                // 递归列出子目录
                list(full_path, level + 1);
            } else {
                printf("📄 %s (%ld bytes)\n", entry->d_name, st.st_size);
            }
        }
    }
    closedir(dir);
}
