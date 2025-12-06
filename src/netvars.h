#pragma once

#include <cstdint>
#include <string>

namespace netvars
{
	uintptr_t get_netvar_address(const std::string& netvar);
}