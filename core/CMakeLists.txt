set(COMPONENT_NAME core)
file(GLOB_RECURSE CORE_SRCS 
    "src/*.c"    # 匹配 C 源文件
    "src/*.cpp"  # 匹配 C++ 源文件
)
set(CORE_INCLUDES "inc")

# 注册组件
idf_component_register(
    SRCS 
        ${CORE_SRCS}
    
    INCLUDE_DIRS 
        ${CORE_INCLUDES}
        "."
    
    REQUIRES 
        esp_http_server
        esp_wifi
        esp_netif
        nvs_flash
        freertos
        json
        joltwallet__littlefs
        sdmmc
        bt
        
     
)