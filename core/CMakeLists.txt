set(COMPONENT_NAME core)
file(GLOB_RECURSE CORE_SRCS 
    "src/*.c"   
    "src/*.cpp"  
)
file(GLOB_RECURSE CORE_COMPONENT_SRCS 
    "component/src/*.c"
    "component/src/*.cpp"

)
set(CORE_INCLUDES "inc")
set(CORE_COMPONENT_INCLUDES "component/inc")
# 注册组件
idf_component_register(
    SRCS 
        ${CORE_SRCS}
        ${CORE_COMPONENT_SRCS}
    
    INCLUDE_DIRS 
        ${CORE_INCLUDES}
        ${CORE_COMPONENT_INCLUDES}
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
        driver
        
     
)