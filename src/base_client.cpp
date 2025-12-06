#include "base_client.h"

#include "mem.h"

client_class_t* c_base_client::get_all_classes()
{
	return mem::call_virtual_fn<client_class_t*, 7>(this);
}