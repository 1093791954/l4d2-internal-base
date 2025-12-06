#include "trace_ray.h"

#include "mem.h"

void c_trace_ray::trace(const ray_t& ray, unsigned int mask, trace_filter_t* filter, trace_t* trace)
{
	return mem::call_virtual_fn<void, 5>(this, ray, 0x46004003, filter, trace);
}