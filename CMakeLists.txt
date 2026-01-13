cmake_minimum_required(VERSION 3.10)
project(PathPlanningEngine)
# 在 project(MapEngine) 之后添加
if(MSVC)
    add_compile_options(/utf-8)
endif()
# 设置C++标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 设置编译选项
if(MSVC)
    add_compile_options(/W4 /O2)
else()
    add_compile_options(-Wall -Wextra -O3)
endif()

# 包含目录
include_directories(${CMAKE_SOURCE_DIR}/include)

# 源文件
set(SOURCES
    src/Graph.cpp
    src/PathFinder.cpp
    src/DataLoader.cpp
    src/MapEngine.cpp
)

# 主程序
add_executable(path_planning_engine
    ${SOURCES}
    src/main.cpp
)

# 测试程序
add_executable(path_planning_test
    ${SOURCES}
    tests/test.cpp
)

# 输出目录
set_target_properties(path_planning_engine PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)

set_target_properties(path_planning_test PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)

# Windows特定设置
if(WIN32)
    # 复制数据文件到输出目录
    add_custom_command(TARGET path_planning_engine POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/data
        ${CMAKE_BINARY_DIR}/bin/data
    )
    
    add_custom_command(TARGET path_planning_test POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/data
        ${CMAKE_BINARY_DIR}/bin/data
    )
endif()

