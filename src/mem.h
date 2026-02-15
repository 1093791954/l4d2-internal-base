#pragma once

// 注意：pch.h 会通过 CMake 的 /FI 选项强制包含

#include <string>

using create_interface_fn = void*(*)(const char*, int*);

namespace mem
{
	template <typename T>
	inline T& read(const uintptr_t addr)
	{
		return *reinterpret_cast<T*>(addr);
	}

	template <typename T>
	inline void write(const uintptr_t addr, T val)
	{
		*reinterpret_cast<T*>(addr) = val;
	}

	template <typename T>
	inline T get_virtual_fn(void* base_class, int index)
	{
		return (*static_cast<T**>(base_class))[index];
	}

	template <typename T, int index, typename... Args>
	constexpr T call_virtual_fn(void* base_class, Args... args)
	{
		using Fn = T(__thiscall*)(void*, decltype(args)...);
		return reinterpret_cast<Fn>(get_virtual_fn<Fn>(base_class, index))(base_class, args...);
	}

	template <typename T>
	inline T get_export(const std::string& module_name, const std::string& function_name)
	{
		return reinterpret_cast<T>(GetProcAddress(GetModuleHandleA(module_name.c_str()), function_name.c_str()));
	}

	template <typename T>
	inline T* get_interface(const std::string& module_name, const std::string& interface_name)
	{
		const auto fn = get_export<create_interface_fn>(module_name, "CreateInterface");

		if (!fn)
			return nullptr;

		return static_cast<T*>(fn(interface_name.c_str(), {}));
	}
}