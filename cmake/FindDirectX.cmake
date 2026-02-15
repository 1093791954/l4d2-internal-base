# FindDirectX.cmake - 查找 DirectX SDK
#
# 使用方法:
#   find_package(DirectX REQUIRED)
#
# 定义变量:
#   DirectX_FOUND        - 是否找到 DirectX
#   DirectX_INCLUDE_DIRS - DirectX 包含目录
#   DirectX_LIBRARIES    - DirectX 库文件

# 查找 DirectX SDK 包含目录
find_path(DirectX_INCLUDE_DIRS
    NAMES d3d9.h d3dx9.h
    PATHS
        "$ENV{DXSDK_DIR}Include"
        "C:/Program Files (x86)/Microsoft DirectX SDK (June 2010)/Include"
        "C:/Program Files/Microsoft DirectX SDK (June 2010)/Include"
        "C:/Program Files (x86)/Microsoft DirectX SDK/Include"
        "C:/Program Files/Microsoft DirectX SDK/Include"
    DOC "DirectX SDK 包含目录"
)

# 查找 DirectX 库目录（x86）
if(CMAKE_SIZEOF_VOID_P EQUAL 8)
    # 64位系统
    set(DirectX_LIB_SEARCH_PATHS
        "$ENV{DXSDK_DIR}Lib/x64"
        "C:/Program Files (x86)/Microsoft DirectX SDK (June 2010)/Lib/x64"
        "C:/Program Files/Microsoft DirectX SDK (June 2010)/Lib/x64"
    )
else()
    # 32位系统
    set(DirectX_LIB_SEARCH_PATHS
        "$ENV{DXSDK_DIR}Lib/x86"
        "C:/Program Files (x86)/Microsoft DirectX SDK (June 2010)/Lib/x86"
        "C:/Program Files/Microsoft DirectX SDK (June 2010)/Lib/x86"
        "C:/Program Files (x86)/Microsoft DirectX SDK (June 2010)/Lib"
        "C:/Program Files/Microsoft DirectX SDK (June 2010)/Lib"
    )
endif()

# 查找 DirectX 库文件
find_library(D3D9_LIBRARY
    NAMES d3d9
    PATHS ${DirectX_LIB_SEARCH_PATHS}
    DOC "Direct3D9 库文件"
)

find_library(D3DX9_LIBRARY
    NAMES d3dx9
    PATHS ${DirectX_LIB_SEARCH_PATHS}
    DOC "D3DX9 库文件"
)

# 设置库列表
set(DirectX_LIBRARIES
    ${D3D9_LIBRARY}
    ${D3DX9_LIBRARY}
)

# 处理标准参数
include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(DirectX
    REQUIRED_VARS
        DirectX_INCLUDE_DIRS
        D3D9_LIBRARY
    FAIL_MESSAGE "找不到 DirectX SDK，将尝试使用 Windows SDK"
)

# 标记高级变量
mark_as_advanced(DirectX_INCLUDE_DIRS DirectX_LIBRARIES D3D9_LIBRARY D3DX9_LIBRARY)
