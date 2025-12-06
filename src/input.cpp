#include "input.h"

#include "ui.h"

extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND h, UINT m, WPARAM w, LPARAM l);
static unsigned long WINAPI wnd_proc_hook(HWND h, UINT m, WPARAM w, LPARAM l)
{
	if ((g::initialised && !g::done))
	{
		g_input.process_message(m, w, l);
		
		if (g_ui.get_menu_state())
		{
			if (ImGui_ImplWin32_WndProcHandler(g_input.get_window(), m, w, l))
				return true;
		}
	}

	return CallWindowProcA(g_input.get_wndproc(), h, m, w, l);
}

void input::initialise(const std::pair<LPCSTR, LPCSTR>& wnd_data)
{
	m_HWND = FindWindowA(wnd_data.first, wnd_data.second);

	if (m_HWND == NULL)
		return;

	m_wndproc = reinterpret_cast<WNDPROC>(SetWindowLongA(m_HWND, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(wnd_proc_hook)));
}

void input::on_hotkey(unsigned int vk, std::function<void()> fn)
{
	m_hotkeys[vk] = fn;
}

void input::process_message(UINT m, WPARAM w, LPARAM l)
{
	process_keybd_message(m, w);
	process_mouse_message(m, w, l);

	switch (m) {
	case WM_MOUSEMOVE: {
		m_mouse_pos_x = LOWORD(l);
		m_mouse_pos_y = HIWORD(l);
		break;
	}
	case WM_MOUSEWHEEL: {
		m_wheel_accumulate += GET_WHEEL_DELTA_WPARAM(w);
		break;
	}
	}
}

void input::process_mouse_message(UINT m, WPARAM w, LPARAM l)
{
	auto state = bs_none;
	auto key = static_cast<int>(w);

	switch (m) {
	case WM_MBUTTONDBLCLK:
	case WM_MBUTTONDOWN:
	case WM_MBUTTONUP: {
		state = m == WM_MBUTTONUP ? bs_up : bs_down;
		key = VK_MBUTTON;
		break;
	}
	case WM_RBUTTONDBLCLK:
	case WM_RBUTTONDOWN:
	case WM_RBUTTONUP: {
		state = m == WM_RBUTTONUP ? bs_up : bs_down;
		key = VK_RBUTTON;
		break;
	}
	case WM_LBUTTONDBLCLK:
	case WM_LBUTTONDOWN:
	case WM_LBUTTONUP: {
		state = m == WM_LBUTTONUP ? bs_up : bs_down;
		key = VK_LBUTTON;
		break;
	}
	case WM_XBUTTONDBLCLK:
	case WM_XBUTTONDOWN:
	case WM_XBUTTONUP: {
		state = m == WM_XBUTTONUP ? bs_up : bs_down;
		key = (HIWORD(w) == XBUTTON1 ? VK_XBUTTON1 : VK_XBUTTON2);
		break;
	}
	default:
		return;
	}

	m_states[key] = (state == bs_up &&
		m_states[key] == bs_down) ? bs_pressed : state;
}

void input::process_keybd_message(UINT m, WPARAM w)
{
	auto state = bs_none;

	switch (m) {
	case WM_KEYDOWN:
	case WM_SYSKEYDOWN: {
		state = bs_down;
		break;
	}
	case WM_KEYUP:
	case WM_SYSKEYUP: {
		state = bs_up;
		break;
	}
	default:
		return;
	}

	auto key = static_cast<int>(w);
	if ((state == bs_up && m_states[key] == bs_down)) {
		m_states[key] = bs_pressed;

		auto& hotkey_callback = m_hotkeys[key];

		if (hotkey_callback)
			hotkey_callback();
	}
	else
	{
		m_states[key] = state;
	}
}

std::string input::virtual_key_to_wstring(unsigned int vk)
{
	std::string k{ "?" };
	auto map{ MapVirtualKeyA(vk, MAPVK_VK_TO_VSC) };

	switch (vk) {
	case 0: {
		return k;
	}
	case VK_LEFT:
	case VK_UP:
	case VK_RIGHT:
	case VK_DOWN:
	case VK_RCONTROL:
	case VK_RMENU:
	case VK_LWIN:
	case VK_RWIN:
	case VK_APPS:
	case VK_PRIOR:
	case VK_NEXT:
	case VK_END:
	case VK_HOME:
	case VK_INSERT:
	case VK_DELETE:
	case VK_DIVIDE:
	case VK_NUMLOCK:
		map |= KF_EXTENDED;
	default:
		char buf[128]{};
		if (GetKeyNameTextA(map << 16, buf, 128))
			k = buf;
	}

	return k;
}

_button_state input::get_key_state(unsigned int vk)
{
	return m_states[vk];
}

HWND input::get_window()
{
	return m_HWND;
}

WNDPROC input::get_wndproc()
{
	return m_wndproc;
}

void input::shutdown()
{
	m_mouse_pos_x = 0;
	m_mouse_pos_x = 0;

	m_wheel_accumulate = 0;

	SetWindowLongA(m_HWND, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(m_wndproc));
	m_wndproc = 0;
}