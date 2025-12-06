#pragma once

#include "util.h"

#define CFG_INT_KEY        "{i32}"
#define CFG_FLOAT_KEY      "{f32}"

#define CFG_BOOL_TRUE_KEY  "{on}"
#define CFG_BOOL_FALSE_KEY "{off}"

namespace config
{
	void load(const std::string& name);
	void save(const std::string& name);
}