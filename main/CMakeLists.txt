file(GLOB SRCS
    "src/*.c"
    "src/*.cpp"
)
# 使用新的ESP-IDF组件注册API
idf_component_register(
    # 主源文件 - 只包含main.cpp，component和core作为独立组件被引用
    SRCS 
        "main.cpp"
        ${SRCS}
        
        
    # 包含目录
    INCLUDE_DIRS 
        "."
        "inc"
    
    # 组件依赖 - 引用其他组件，包括我们自定义的component和core
    REQUIRES 
        component
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