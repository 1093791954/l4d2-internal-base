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

// 射线检测掩码定义
#define CONTENTS_SOLID			0x1		// 固体物体
#define CONTENTS_MOVEABLE		0x4000	// 可移动物体
#define CONTENTS_MONSTER		0x2000000	// 怪物/AI
#define CONTENTS_WINDOW			0x2		// 窗户
#define CONTENTS_DEBRIS			0x4000000	// 碎片
#define CONTENTS_HITBOX			0x40000000	// 命中框

#define MASK_SHOT				(CONTENTS_SOLID|CONTENTS_MOVEABLE|CONTENTS_MONSTER|CONTENTS_WINDOW|CONTENTS_DEBRIS|CONTENTS_HITBOX)

struct ray_t {
	ray_t(const vec3& src, const vec3& dest) {
		delta = vec_aligned(dest - src);
		is_swept = (delta.length_sqr() != 0.0f);
		extents.init();
		world_axis_transform = NULL;
		is_ray = true;
		start_offset.init();
		start = src;
	}

	vec_aligned start;
	vec_aligned delta;
	vec_aligned start_offset;
	vec_aligned extents;
	const matrix3x4_t* world_axis_transform;
	bool is_ray;
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