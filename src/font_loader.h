#pragma once

#include <windows.h>
#include <cstdint>
#include <vector>

struct font_resource_t {
	int m_index{};
	unsigned char* m_data{};
	std::uint32_t m_data_len{};
};

struct font_loader {
	void initialise(const std::vector<font_resource_t>& fonts);
	void shutdown(const std::vector<font_resource_t>& fonts);

private:
	HANDLE m_handles[32]{};
	DWORD  m_num{};
};

inline font_loader g_font_loader{};