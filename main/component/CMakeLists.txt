# 定义组件名称（可选，但有助于调试）
set(COMPONENT_NAME component)

# 添加源文件
file(GLOB COMPONENT_SRCS
    "src/*.c"
    "src/*.cpp"
)

# 使用新的ESP-IDF组件注册API
idf_component_register(
    # 源文件
    SRCS
        ${COMPONENT_SRCS}
    
    # 包含目录 - 暴露给其他组件使用
    INCLUDE_DIRS
        "inc"
    
    # 组件依赖 - 其他组件将自动链接到这个组件
    REQUIRES
        core
)

# 设置C++17标准支持
set_target_properties(${COMPONENT_LIB} PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
)