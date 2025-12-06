#pragma once

#define CLIENT_ENTITY_INTERFACE_VERSION "VClientEntityList003"

#include "base_entity.h"
#include "mem.h"

class c_client_entity_list {
public:
	template <class T>
	/*
	* IClientEntityList::GetHighestEntityIndex()
	* Get specified entity by index.
	*/
	T get(int index) {
		return mem::call_virtual_fn<T, 3>(this, index);
	}

	/*
	* IClientEntityList::GetHighestEntityIndex()
	* Returns highest index actually used.
	*/
	int get_highest_entity_index() {
		return mem::call_virtual_fn<int, 6>(this);
	}

	/*
	* IClientEntityList::GetMaxEntities()
	* Entity list size.
	*/
	int get_max_entities() {
		return mem::call_virtual_fn<int, 8>(this);
	}
};