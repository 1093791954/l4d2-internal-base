#pragma once

#define _CRT_SECURE_NO_WARNINGS

#include "common.h"
#include "globals.h"

#include <string>
#include <windows.h>
#include <thread>

enum _wait_for_module {
	WFM_OK,
	WFM_TIMEOUT,
};

struct mod_t {
	mod_t(const int32_t reason) : m_reason_to_call(reason) {};

	void in(const int32_t reason, std::function<void(void)> fn) {
		if (m_reason_to_call == reason) fn();
	}

private:
	int32_t m_reason_to_call{};
};

namespace util
{
	// Waits for a specific module to be loaded within a given timeout period.
	// Returns WFM_OK if the module is loaded before timeout, or WFM_TIMEOUT if timeout occurs.
	_wait_for_module wait_for_mod(const _module_list module_index, int ms = 800);

	// Encrypts or decrypts a string using XOR operation with a given key.
	// Since XOR is symmetric, the same function can be used for both encryption and decryption.
	std::string xor_encrypt_decrypt(const std::string& data, const std::string& key);

	// Removes all occurrences of specific characters from a string.
	std::string remove_chars_from_string(std::string string, const std::string& chars);

	// Gets the current local time as a string in HH:MM:SS format.
	std::string get_current_time();

	// Calculates the elapsed time in milliseconds since the provided start time.
	long long get_elapsed_time(const std::chrono::steady_clock::time_point start_time);
}