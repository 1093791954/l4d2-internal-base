cmake_minimum_required(VERSION 3.15)

# 禁用 vcpkg 自动集成（避免头文件冲突）
set(CMAKE_TOOLCHAIN_FILE "" CACHE STRING "Toolchain file" FORCE)
set(VCPKG_TARGET_TRIPLET "" CACHE STRING "" FORCE)

project(l4d2-internal-base VERSION 1.0.0 LANGUAGES C CXX)

# 设置 C++ 标准
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# 添加自定义 CMake 模块路径
list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")

# 设置输出目录
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib)

# 查找 DirectX SDK（可选）
find_package(DirectX QUIET)

# 如果找不到 DirectX SDK，尝试使用 Windows SDK
if(NOT DirectX_FOUND)
    message(STATUS "DirectX SDK 未找到，尝试使用 Windows SDK")
    set(DirectX_INCLUDE_DIRS "")
    set(DirectX_LIBRARIES "")
endif()

# 收集源文件
# 项目源文件
file(GLOB_RECURSE PROJECT_SOURCES
    "${CMAKE_SOURCE_DIR}/src/*.cpp"
)

file(GLOB_RECURSE PROJECT_HEADERS
    "${CMAKE_SOURCE_DIR}/src/*.h"
)

# ImGui 源文件
file(GLOB IMGUI_SOURCES
    "${CMAKE_SOURCE_DIR}/3rdparty/imgui/*.cpp"
)

file(GLOB IMGUI_BACKENDS_SOURCES
    "${CMAKE_SOURCE_DIR}/3rdparty/imgui/backends/*.cpp"
)

# MinHook 源文件
file(GLOB_RECURSE MINHOOK_SOURCES
    "${CMAKE_SOURCE_DIR}/3rdparty/minhook/src/*.c"
)

# 资源头文件
file(GLOB RESOURCES_HEADERS
    "${CMAKE_SOURCE_DIR}/3rdparty/resources/*.hpp"
)

# 合并所有源文件
set(ALL_SOURCES
    ${PROJECT_SOURCES}
    ${PROJECT_HEADERS}
    ${IMGUI_SOURCES}
    ${IMGUI_BACKENDS_SOURCES}
    ${MINHOOK_SOURCES}
    ${RESOURCES_HEADERS}
)

# 创建 DLL
add_library(${PROJECT_NAME} SHARED ${ALL_SOURCES})

# 设置包含目录
target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/3rdparty/imgui
    ${CMAKE_SOURCE_DIR}/3rdparty/imgui/backends
    ${CMAKE_SOURCE_DIR}/3rdparty/minhook/include
    ${CMAKE_SOURCE_DIR}/3rdparty/resources
    ${DirectX_INCLUDE_DIRS}
)

# 链接库
target_link_libraries(${PROJECT_NAME} PRIVATE
    ${DirectX_LIBRARIES}
)

# 编译选项
if(MSVC)
    # 定义 Windows 版本（在编译前）
    target_compile_definitions(${PROJECT_NAME} PRIVATE
        WIN32_LEAN_AND_MEAN
        NOMINMAX
        _CRT_SECURE_NO_WARNINGS
    )

    target_compile_options(${PROJECT_NAME} PRIVATE
        /W0           # 关闭所有警告，与原项目一致
        /permissive   # 禁用严格一致性模式
    )

    # 强制包含 pch.h 到 C++ 编译单元（不能应用于 MinHook 的 C 文件）
    set_source_files_properties(${PROJECT_SOURCES} ${IMGUI_SOURCES} ${IMGUI_BACKENDS_SOURCES}
        PROPERTIES COMPILE_OPTIONS "/FI${CMAKE_SOURCE_DIR}/src/pch.h"
    )

    # 启用预编译头（可选，可加速编译）
    # set_target_properties(${PROJECT_NAME} PROPERTIES
    #     PREFIX_HEADER "${CMAKE_SOURCE_DIR}/src/pch.h"
    #     PRECOMPILED_HEADER "${CMAKE_SOURCE_DIR}/src/pch.h"
    #     PRECOMPILED_HEADER_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/pch"
    # )

    # 设置 C++ 标准
    set_target_properties(${PROJECT_NAME} PROPERTIES
        CXX_STANDARD 20
        CXX_STANDARD_REQUIRED ON
    )

    # Release 配置特定选项
    target_compile_definitions(${PROJECT_NAME} PRIVATE
        $<$<CONFIG:Release>:NDEBUG>
    )
endif()

# 设置平台为 x86
set_target_properties(${PROJECT_NAME} PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/lib"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin"
)

# 配置特定的输出目录
set_target_properties(${PROJECT_NAME} PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY_DEBUG "${CMAKE_SOURCE_DIR}/bin/Debug"
    LIBRARY_OUTPUT_DIRECTORY_DEBUG "${CMAKE_SOURCE_DIR}/bin/Debug"
    RUNTIME_OUTPUT_DIRECTORY_DEBUG "${CMAKE_SOURCE_DIR}/bin/Debug"
    ARCHIVE_OUTPUT_DIRECTORY_RELEASE "${CMAKE_SOURCE_DIR}/bin/Release"
    LIBRARY_OUTPUT_DIRECTORY_RELEASE "${CMAKE_SOURCE_DIR}/bin/Release"
    RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_SOURCE_DIR}/bin/Release"
)

message(STATUS "Project: ${PROJECT_NAME}")
message(STATUS "DirectX Include: ${DirectX_INCLUDE_DIRS}")
message(STATUS "DirectX Libraries: ${DirectX_LIBRARIES}")
message(STATUS "Build Type: ${CMAKE_BUILD_TYPE}")
