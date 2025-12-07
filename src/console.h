#pragma once

struct console {
	void initialise();
	void shutdown();
};

inline console g_console{};