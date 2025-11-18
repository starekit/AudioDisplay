// #pragma once

// #include <stdio.h>
// #include <stdlib.h>
// #include <string.h>
// #include "freertos/FreeRTOS.h"
// #include "freertos/task.h"
// #include "freertos/event_groups.h"
// #include "esp_system.h"
// #include "esp_log.h"
// #include "nvs_flash.h"
// #include "esp_bt.h"
// #include "esp_gap_ble_api.h"
// #include "esp_gatts_api.h"
// #include "esp_bt_main.h"
// #include "esp_bt_device.h"
// #include <esp_bit_defs.h>




// #define GATTS_TAG "BLE_SERVER"
// #define DEVICE_NAME "ESP32_BLE_SERVER"
// #define SERVICE_UUID 0x00FF
// #define CHAR_UUID    0xFF01




// // 声明回调函数
// void gap_event_handler(esp_gap_ble_cb_event_t event, esp_ble_gap_cb_param_t *param);
// void gatts_event_handler(esp_gatts_cb_event_t event, esp_gatt_if_t gatts_if, esp_ble_gatts_cb_param_t *param);

// // 声明blue函数
// void blue(void);

// namespace core{

//     class Bluetooth{



//         public:
//             Bluetooth();
//             ~Bluetooth();

//         public:
//             void begin();
//             void scan();
//             void link();

//     };


// }