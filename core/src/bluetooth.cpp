#include "bluetooth.h"
using namespace core;

// 实现回调函数
static esp_ble_adv_params_t adv_params = {
    .adv_int_min        = 0x20,
    .adv_int_max        = 0x40,
    .adv_type           = ADV_TYPE_IND,
    .own_addr_type      = BLE_ADDR_TYPE_PUBLIC,
    .channel_map        = ADV_CHNL_ALL,
    .adv_filter_policy = ADV_FILTER_ALLOW_SCAN_ANY_CON_ANY,
};

static uint8_t char_value[20] = {0x00};

void gap_event_handler(esp_gap_ble_cb_event_t event, esp_ble_gap_cb_param_t *param)
{
    switch (event) {
    case ESP_GAP_BLE_ADV_DATA_SET_COMPLETE_EVT:
        ESP_LOGI(GATTS_TAG, "ADV data set complete");
        esp_ble_gap_start_advertising(&adv_params);
        break;
    case ESP_GAP_BLE_ADV_START_COMPLETE_EVT:
        if (param->adv_start_cmpl.status != ESP_BT_STATUS_SUCCESS) {
            ESP_LOGE(GATTS_TAG, "Advertising start failed");
        } else {
            ESP_LOGI(GATTS_TAG, "Advertising started successfully");
        }
        break;
    default:
        break;
    }
}

void gatts_event_handler(esp_gatts_cb_event_t event, esp_gatt_if_t gatts_if, esp_ble_gatts_cb_param_t *param){
    switch (event) {
    case ESP_GATTS_REG_EVT: {
        ESP_LOGI(GATTS_TAG, "ESP_GATTS_REG_EVT");
        
        esp_ble_gap_set_device_name(DEVICE_NAME);
        
        uint8_t adv_service_uuid[16] = {0};
        adv_service_uuid[15] = SERVICE_UUID & 0xff;
        adv_service_uuid[14] = (SERVICE_UUID >> 8) & 0xff;
        
        esp_ble_adv_data_t adv_data = {
            .set_scan_rsp = false,
            .include_name = true,
            .include_txpower = true,
            .min_interval = 0x0006,
            .max_interval = 0x0010,
            .appearance = 0x00,
            .manufacturer_len = 0,
            .p_manufacturer_data = NULL,
            .service_data_len = 0,
            .p_service_data = NULL,
            .service_uuid_len = sizeof(adv_service_uuid),
            .p_service_uuid = adv_service_uuid,
            .flag = (ESP_BLE_ADV_FLAG_GEN_DISC | ESP_BLE_ADV_FLAG_BREDR_NOT_SPT),
        };
        
        esp_ble_gap_config_adv_data(&adv_data);
        break;
    }
        
    case ESP_GATTS_READ_EVT:
        ESP_LOGI(GATTS_TAG, "ESP_GATTS_READ_EVT");
        break;
        
    case ESP_GATTS_WRITE_EVT:
        ESP_LOGI(GATTS_TAG, "ESP_GATTS_WRITE_EVT");
        break;
        
    case ESP_GATTS_MTU_EVT:
        ESP_LOGI(GATTS_TAG, "ESP_GATTS_MTU_EVT, MTU %d", param->mtu.mtu);
        break;
        
    case ESP_GATTS_CONNECT_EVT:
        ESP_LOGI(GATTS_TAG, "ESP_GATTS_CONNECT_EVT");
        break;
        
    case ESP_GATTS_DISCONNECT_EVT:
        ESP_LOGI(GATTS_TAG, "ESP_GATTS_DISCONNECT_EVT");
        break;
        
    case ESP_GATTS_CREATE_EVT:
        ESP_LOGI(GATTS_TAG, "ESP_GATTS_CREATE_EVT");
        break;
        
    case ESP_GATTS_SEND_SERVICE_CHANGE_EVT:
        ESP_LOGI(GATTS_TAG, "ESP_GATTS_SEND_SERVICE_CHANGE_EVT, status=%d", param->service_change.status);
        break;
        
    default:
        ESP_LOGD(GATTS_TAG, "Unhandled event: %d", event);
        break;
    }
}

void blue(void){
    // 初始化 NVS
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);

    // 初始化蓝牙控制器
    ESP_ERROR_CHECK(esp_bt_controller_mem_release(ESP_BT_MODE_CLASSIC_BT));
    
    esp_bt_controller_config_t bt_cfg = BT_CONTROLLER_INIT_CONFIG_DEFAULT();
    ret = esp_bt_controller_init(&bt_cfg);
    if (ret) {
        ESP_LOGE(GATTS_TAG, "%s initialize controller failed: %s\n", __func__, esp_err_to_name(ret));
        return;
    }

    ret = esp_bt_controller_enable(ESP_BT_MODE_BLE);
    if (ret) {
        ESP_LOGE(GATTS_TAG, "%s enable controller failed: %s\n", __func__, esp_err_to_name(ret));
        return;
    }

    // 初始化 Bluedroid
    ret = esp_bluedroid_init();
    if (ret) {
        ESP_LOGE(GATTS_TAG, "%s init bluedroid failed: %s\n", __func__, esp_err_to_name(ret));
        return;
    }

    ret = esp_bluedroid_enable();
    if (ret) {
        ESP_LOGE(GATTS_TAG, "%s enable bluedroid failed: %s\n", __func__, esp_err_to_name(ret));
        return;
    }

    // 注册 GATT 和 GAP 回调
    ret = esp_ble_gatts_register_callback(gatts_event_handler);
    if (ret) {
        ESP_LOGE(GATTS_TAG, "gatts register error, error code = %x", ret);
        return;
    }
    
    ret = esp_ble_gap_register_callback(gap_event_handler);
    if (ret) {
        ESP_LOGE(GATTS_TAG, "gap register error, error code = %x", ret);
        return;
    }

    // 注册 GATT 应用
    ret = esp_ble_gatts_app_register(0);
    if (ret) {
        ESP_LOGE(GATTS_TAG, "gatts app register error, error code = %x", ret);
        return;
    }

    ESP_LOGI(GATTS_TAG, "BLE Server Started");
}

Bluetooth::Bluetooth(){

}
Bluetooth::~Bluetooth(){
    
}

void Bluetooth::begin(){

}
void Bluetooth::scan(){

}
