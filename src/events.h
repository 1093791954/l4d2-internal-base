#pragma once

#include "game_event_listener2.h"

struct game_event_manager : public c_game_event_listener2 {
	void initialise();
	void shutdown();
private:
	void fire_game_event(c_game_event* _event);
};

inline game_event_manager g_event_manager{};