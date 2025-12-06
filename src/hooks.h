#pragma once

#include "common.h"
#include "globals.h"
#include "ui.h"
#include "sdk.h"

#include <mutex>
#include <MinHook.h>

enum _hk_list {
	HK_PRESENT,
	HK_RESET,
	HK_CREATEMOVE,
	HK_PAINTTRAVERSE,
	HK_GETSCREENASPECTRATIO,
	HK_ISGAMEPAUSED,
	maxHooks,
};

struct hook_t {
	template <typename T, int index>
	void hook(void* addr, void* target, void** orig) {
		if (m_hooked)
			return;

		m_index = index;
		m_addr = addr;
		m_src = mem::get_virtual_fn<T*>(m_addr, m_index);
		m_target = target;
		m_orig = orig;

		MH_STATUS status{};
		
		status = MH_CreateHook(m_src, m_target, m_orig);
		status = MH_EnableHook(m_src);

		m_hooked = true;
	}

	int get_index() {
		return m_index;
	}

	bool is_hooked() {
		return m_hooked;
	}
	
	void unhook() {
		if (!m_hooked)
			return;

		MH_STATUS status{};

		status = MH_DisableHook(m_src);
		status = MH_RemoveHook(m_src);

		m_hooked = false;
	}

private:
	int    m_index{};
	void*  m_addr{};
	void*  m_src{};
	void*  m_target{};
	void** m_orig{};
	bool   m_hooked{};
};

struct hooks {
	void initialise();
	void shutdown();

private:
	hook_t m_hooks[16]{};
};

inline hooks g_hooks{};