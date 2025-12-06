#pragma once

#include <string>
#include <vector>
#include <variant>
#include <optional>

using varType = std::variant<bool, int, float>;
using vars_t = std::vector<std::pair<std::string, varType>>;

struct vars {
	void initialise();
	void set(const std::string& key, const varType& value);

	template <typename T>
	std::optional<T> get_as(const std::string& key)
	{
		for (const auto& v : m_map)
		{
			if (v.first == key)
			{
				std::optional<varType> opt{ v.second };

				if (opt.has_value()) {
					if (const auto val = std::get_if<T>(&opt.value())) {
						return *val;
					}
				}
			}
		}

		return std::nullopt;
	}

	vars_t get_map();

	void shutdown();

private:
	vars_t m_map{};
};

inline vars g_vars{};