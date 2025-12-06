#include "model_info.h"

#include "mem.h"

model_t* c_model_info::get_model(int index)
{
	return mem::call_virtual_fn<model_t*, 1>(this, index);
}

int	c_model_info::get_model_index(const char* name)
{
	return mem::call_virtual_fn<int, 2>(this, name);
}

char* c_model_info::get_model_name(model_t* model)
{
	return mem::call_virtual_fn<char*, 3>(this, model);
}