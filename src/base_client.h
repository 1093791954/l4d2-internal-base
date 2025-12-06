#pragma once

#define CLIENT_INTERFACE_VERSION "VClient016"

#include "client_class.h"

class c_base_client {
public:
	/*
	* IBaseClientDLL::GetAllClasses()
	* Request a pointer to the list of client datatable classes.
	*/
	client_class_t* get_all_classes();
};