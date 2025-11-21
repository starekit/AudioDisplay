#pragma once

#include <stdio.h>
#include "esp_log.h"

#define LOG_COLOR_E "\033[1;31m"  // 红色错误
#define LOG_COLOR_W "\033[1;33m"  // 黄色警告  
#define LOG_COLOR_I "\033[1;32m"  // 绿色信息
#define LOG_COLOR_D "\033[1;34m"  // 蓝色调试
#define LOG_COLOR_V "\033[1;35m"  // 白色详细
#define LOG_RESET   "\033[0m"


// 修复后的宏，使用 %lu 格式
#define LOGE(tag, format, ...) \
    printf(LOG_COLOR_E "E (%lu) %s: " format LOG_RESET "\n", \
           esp_log_timestamp(), tag, ##__VA_ARGS__)

#define LOGW(tag, format, ...) \
    printf(LOG_COLOR_W "W (%lu) %s: " format LOG_RESET "\n", \
           esp_log_timestamp(), tag, ##__VA_ARGS__)

#define LOGI(tag, format, ...) \
    printf(LOG_COLOR_I "I (%lu) %s: " format LOG_RESET "\n", \
           esp_log_timestamp(), tag, ##__VA_ARGS__)

#define LOGD(tag, format, ...) \
    printf(LOG_COLOR_D "D (%lu) %s: " format LOG_RESET "\n", \
           esp_log_timestamp(), tag, ##__VA_ARGS__)

#define LOGV(tag, format, ...) \
    printf(LOG_COLOR_V "V (%lu) %s: " format LOG_RESET "\n", \
           esp_log_timestamp(), tag, ##__VA_ARGS__)