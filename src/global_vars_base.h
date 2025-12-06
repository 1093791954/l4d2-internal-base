#pragma once

#define TICK_INTERVAL			( g_l4d2.m_player_info_mgr->get_globals()->interval_per_tick )
#define TIME_TO_TICKS(time)		( static_cast<int>(0.5f + time / TICK_INTERVAL )
#define TICKS_TO_TIME(time)		( TICK_INTERVAL * (time) )
#define ROUND_TO_TICKS(time)	( TICK_INTERVAL * TIME_TO_TICKS(time) )
#define TICK_NEVER_THINK		( -1 )

class c_global_vars_base {
public:
	float real_time;
	int frame_count;
	float absolute_frame_time;
	float cur_time;
	float frame_time;
	int max_clients;
	int tick_count;
	float interval_per_tick;
	float interpolation_amount;
	int sim_ticks_this_frame;
	int network_protocol;
	void* save_data;
};