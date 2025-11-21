# 设置组件名称
set(COMPONENT_NAME core)

# 添加源文件
file(GLOB CORE_SRCS
    "src/*.c"
    "src/*.cpp"
)

# 使用新的ESP-IDF组件注册API
idf_component_register(
    # 源文件
    SRCS 
        ${CORE_SRCS}
    
    # 包含目录 - 确保inc目录被正确添加
    INCLUDE_DIRS 
        "inc"
        "."
    
    # 组件依赖
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
        driver
        
)

# 添加C++11支持（如果需要）
set_target_properties(${COMPONENT_LIB} PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
)