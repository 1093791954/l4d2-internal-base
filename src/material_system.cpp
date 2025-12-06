#include "material_system.h"

c_material* c_material_system::find_material(char const* material_name, const char* group_name, bool complain, const char* complain_prefix)
{
	return mem::call_virtual_fn<c_material*, 67>(this, material_name, group_name, complain, complain_prefix);
}

material_handle_t c_material_system::first_material()
{
	return mem::call_virtual_fn<material_handle_t, 68>(this);
}

material_handle_t c_material_system::next_material(material_handle_t handle)
{
	return mem::call_virtual_fn<material_handle_t, 69>(this, handle);
}

material_handle_t c_material_system::invalid_material()
{
	return mem::call_virtual_fn<material_handle_t, 70>(this);
}

c_material* c_material_system::get_material(material_handle_t handle)
{
	return mem::call_virtual_fn<c_material*, 71>(this, handle);
}