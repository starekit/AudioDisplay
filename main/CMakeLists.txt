# //idf_component_register(SRCS "main.c"
# //                    INCLUDE_DIRS ".")

                    
set(MANAGE_INCLUDE "manage/inc")
file(GLOB_RECURSE MANAGE_SRCS 
    "manage/src/*.c"    # 匹配 C 源文件
    "manage/src/*.cpp"  # 匹配 C++ 源文件
)
idf_component_register(
    SRCS 
        "main.cpp"
        ${MANAGE_SRCS}
    
    INCLUDE_DIRS 
        "."
        ${MANAGE_INCLUDE}
    
    
    REQUIRES 
        core
        esp_http_server
        esp_http_client
        nvs_flash
        esp_wifi
        esp_event
        esp_netif
        esp_partition
        freertos
        esp_timer
        lwip
        mbedtls

    
    

)