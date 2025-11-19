set(INCLUDES "inc")
file(GLOB_RECURSE SRCS 
    "src/*.c"    # 匹配 C 源文件
    "src/*.cpp"  # 匹配 C++ 源文件
)
idf_component_register(
    SRCS 
        "main.cpp"
        ${SRCS}
    
    INCLUDE_DIRS 
        "."
        ${INCLUDES}
    
    
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