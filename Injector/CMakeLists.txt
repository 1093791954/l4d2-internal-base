cmake_minimum_required(VERSION 3.15)
project(DLL_Injector LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 添加源文件
add_executable(injector
    injector.cpp
)

# Windows 链接库
target_link_libraries(injector PRIVATE
    user32
    psapi
)

# 编译定义
target_compile_definitions(injector PRIVATE
    WIN32_LEAN_AND_MEAN
    NOMINMAX
    UNICODE
    _UNICODE
    _CRT_SECURE_NO_WARNINGS
)

# MSVC 特定设置
if(MSVC)
    target_compile_options(injector PRIVATE
        /W0
        /permissive
    )

    # 设置输出目录到项目 bin 目录
    set_target_properties(injector PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin"
        RUNTIME_OUTPUT_DIRECTORY_DEBUG "${CMAKE_SOURCE_DIR}/bin/Debug"
        RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_SOURCE_DIR}/bin/Release"
    )

    # 设置为 32 位程序（与目标 DLL 一致）
    # 注意：如果需要注入 64 位进程，需要单独编译 64 位版本
endif()
