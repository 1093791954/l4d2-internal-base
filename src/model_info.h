#pragma once

#define MODEL_INFO_INTERFACE_VERSION "VModelInfoClient004"

#include "model.h"

class c_model_info {
public:
	/*
	* IModelInfo::GetModel(int)
	* Returns model_t* pointer for a model given a precached or dynamic model index.
	*/
	model_t* get_model(int index);

	/*
	* IModelInfo::GetModelIndex(string)
	* Returns index of model by name for precached or known dynamic models.
	* Does not adjust reference count for dynamic models.
	*/
	int	get_model_index(const char* name);

	/*
	* IModelInfo::GetModelName(model_t)
	* Returns name of model.
	*/
	char* get_model_name(model_t* model);
};