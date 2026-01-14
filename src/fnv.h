#pragma once

#include <iostream>

template <typename S>
struct fnv_internal;

template <typename S>
struct fnv1a;

template <>
struct fnv_internal<uint32_t> {
	constexpr static uint32_t default_offset_basis = 0x811C9DC5;
	constexpr static uint32_t prime = 0x01000193;
};

/* fnv1a<uint32_t>::hash - calculated at runtime hash of given string but if you define a variable as constexpr it will also be calculated at compiletime */
/* fnv1a<uint32_t>::hash_const - calculated at compiletime hash of given string */

template <>
struct fnv1a<uint32_t> : private fnv_internal<uint32_t>
{
	constexpr static uint32_t fnv1a<uint32_t>::hash(char const* string, const uint32_t val = default_offset_basis)
	{
		return (string[0] == '\0') ? val : hash(&string[1], (val ^ uint32_t(string[0])) * prime);
	}

	constexpr static uint32_t fnv1a<uint32_t>::hash(wchar_t const* string, const uint32_t val = default_offset_basis)
	{
		return (string[0] == L'\0') ? val : hash(&string[1], (val ^ uint32_t(string[0])) * prime);
	}

	consteval static uint32_t fnv1a<uint32_t>::hash_const(char const* string, const uint32_t val = default_offset_basis)
	{
		return (string[0] == '\0') ? val : hash_const(&string[1], (val ^ uint32_t(string[0])) * prime);
	}

	consteval static uint32_t fnv1a<uint32_t>::hash_const(wchar_t const* string, const uint32_t val = default_offset_basis)
	{
		return (string[0] == L'\0') ? val : hash_const(&string[1], (val ^ uint32_t(string[0])) * prime);
	}
};

using fnv = fnv1a<uint32_t>;