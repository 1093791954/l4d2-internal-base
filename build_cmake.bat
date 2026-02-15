@echo off
REM L4D2 Internal Base CMake 构建脚本

setlocal enabledelayedexpansion

echo ========================================
echo L4D2 Internal Base - CMake Build Script
echo ========================================
echo.

REM 设置构建类型
set BUILD_TYPE=Release
if "%1"=="debug" set BUILD_TYPE=Debug
if "%1"=="Debug" set BUILD_TYPE=Debug

REM 设置架构 (默认 x86)
set ARCH=x86

REM 设置生成器
set GENERATOR="Visual Studio 17 2022"

REM 检查是否已存在构建目录
if exist build-cmake (
    echo [INFO] 构建目录已存在，清理中...
    rmdir /s /q build-cmake
)

echo [INFO] 创建构建目录...
mkdir build-cmake

echo [INFO] 配置 CMake 项目...
echo [INFO] 构建类型: %BUILD_TYPE%
echo [INFO] 架构: %ARCH%
echo.

cd build-cmake

REM 配置项目
cmake -G %GENERATOR% -A %ARCH% ^
    -DCMAKE_BUILD_TYPE=%BUILD_TYPE% ^
    -DCMAKE_INSTALL_PREFIX=../install ^
    ..

if %ERRORLEVEL% NEQ 0 (
    echo.
    echo [ERROR] CMake 配置失败！
    echo [ERROR] 请检查是否安装了 DirectX SDK 和 Visual Studio。
    cd ..
    exit /b %ERRORLEVEL%
)

echo.
echo [INFO] 开始编译项目 (%BUILD_TYPE%)...
echo.

REM 编译项目
cmake --build . --config %BUILD_TYPE% --clean-first

if %ERRORLEVEL% NEQ 0 (
    echo.
    echo [ERROR] 编译失败！
    cd ..
    exit /b %ERRORLEVEL%
)

echo.
echo ========================================
echo [SUCCESS] 编译完成！
echo ========================================
echo.
echo 输出目录: %CD%\bin\%BUILD_TYPE%
echo.

REM 列出生成的文件
if exist bin\%BUILD_TYPE%\*.dll (
    echo [INFO] 生成的 DLL 文件:
    dir /b bin\%BUILD_TYPE%\*.dll
    echo.
)

cd ..

endlocal
