#pragma once
#include <iostream>
#include <memory>
#include <string>
#include <stdio.h>
#include <dirent.h>
#include <functional>
#include <sys/stat.h>

#include "nvs.h"
#include "nvs_flash.h"
#include "sdkconfig.h"

#include "esp_log.h"
#include "esp_err.h"
#include "esp_event.h"
#include "esp_netif.h"
#include "esp_system.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"