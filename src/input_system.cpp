#include "input_system.h"

#include "mem.h"

void c_input_system::enable_input(bool v)
{
	return mem::call_virtual_fn<void, 2>(this);
}

bool c_input_system::is_button_down(_button_code code)
{
	return mem::call_virtual_fn<bool, 6>(this, code);
}