#pragma once

#define ENGINE_TRACE_INTERFACE_VERSION "EngineTraceClient003"

#include "matrix.h"
#include "base_player.h"

enum _trace_type {
	TRACE_EVERYTHING = 0,
	TRACE_WORLD_ONLY,
	TRACE_ENTITIES_ONLY,
	TRACE_EVERYTHING_FILTER_PROPS,
};

struct ray_t {
	ray_t(const vec3& src, const vec3& dest) : start(src), delta(dest - src) {
		is_swept = delta.x || delta.y || delta.z;
	}

	vec3 start;
	float u;
	vec3 delta;
	unsigned char u2[40]{};
	bool is_ray{ true };
	bool is_swept;
};

struct trace_filter_t {
	trace_filter_t(const c_base_entity* entity) : skip{ entity } {}

	virtual bool should_hit_entity(c_base_entity* entity, int) {
		return entity != skip;
	}

	virtual int get_trace_type() const {
		return TRACE_EVERYTHING;
	}

	const void* skip;
};

struct trace_t {
	vec3 m_start;
	vec3 m_end;

	struct {
		vec3 m_normal;
		float m_distance;
		char m_type;
		char m_signbits;
		short u;
	} plane_t;

	float m_fraction;
	int	m_contents;
	short m_dispflags;
	bool m_allsolid;
	bool m_startsolid;
	float m_fractionleftsolid;
	const char* m_name;
	short m_surfaceprops;
	short m_flags;
	int m_hitgroup;
	short m_physicsbone;
	c_base_entity* m_entity;
	int m_hitbox;
};

class c_trace_ray {
public:
	void trace(const ray_t& ray, unsigned int mask, trace_filter_t* filter, trace_t* trace);
};