cmake_minimum_required(VERSION 3.5)

set(PROJECT_NAME AudioDisplay)

# 包含ESP-IDF项目CMake文件
include($ENV{IDF_PATH}/tools/cmake/project.cmake)

# 设置额外的组件目录 - 这些是相对于项目根目录的路径
set(EXTRA_COMPONENT_DIRS
    main/component
    main/core
)

# 构建项目
project(${PROJECT_NAME})