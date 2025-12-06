#include "game_event_listener2.h"

#include "mem.h"

bool c_game_event_manager2::add_listener(c_game_event_listener2* listener, const char* name, bool server_side)
{
	return mem::call_virtual_fn<bool, 3>(this, listener, name, server_side);
}

int c_game_event_manager2::remove_listener(c_game_event_listener2* listener)
{
	return mem::call_virtual_fn<int, 5>(this, listener);
}