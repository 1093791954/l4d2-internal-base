#pragma once

// ============================================================
// 预编译头��件 (Precompiled Header)
// ============================================================
// 关键：Windows.h 必须在任何 C++ 标准库头文件之前包含！
// 否则 Windows 的宏会破坏标准库的命名空间
//
// 此文件必须通过 CMake 的 /FI 选项强制包含到所有编译单元
// ============================================================

// 第一步：定义 Windows 宏（必须在包含任何 Windows 头文件之前）
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#ifndef NOMINMAX
#define NOMINMAX
#endif
#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif

// 第二步：包含 Windows 头文件
#include <Windows.h>

// 第三步：包含 C 运行时头文件（C 风格）
#include <cmath>
#include <cstdlib>
#include <cstdio>
#include <cstring>

// 第四步：包含 C++ 标准库头文件
#include <algorithm>
#include <string>
#include <vector>
#include <map>
#include <set>
#include <fstream>
#include <sstream>
#include <iostream>
#include <chrono>
#include <functional>
#include <memory>
#include <thread>
#include <filesystem>
#include <array>
#include <optional>
#include <mutex>

// 第五步：包含 DirectX 头文件
#include <d3d9.h>
#include <d3dx9.h>
