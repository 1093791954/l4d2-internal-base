#include "config.h"

#include "vars.h"

#include <fstream>
#include <sstream>

void config::load(const std::string& name)
{
	std::ifstream ifs{ name, std::ios::binary };

	if (!ifs.is_open())
		return;

	std::string encrypted{ (std::istreambuf_iterator<char>(ifs)), std::istreambuf_iterator<char>() };
	std::string decrypted{ util::xor_encrypt_decrypt(encrypted, name) };

	std::istringstream iss{ decrypted };
	std::string line{};

	while (std::getline(iss, line)) {
		const auto equal_pos = line.find('=');

		if (equal_pos == std::string::npos)
			continue;

		const auto key = line.substr(0, equal_pos);
		const auto value = line.substr(equal_pos + 1);

		if (value == CFG_BOOL_TRUE_KEY)
			g_vars.set(key, true);

		else if (value == CFG_BOOL_FALSE_KEY)
			g_vars.set(key, false);

		else
		{
			if (value.find(CFG_INT_KEY) != std::string::npos) {
				g_vars.set(key, std::stoi(util::remove_chars_from_string(value, CFG_INT_KEY)));
			}
			else if (value.find(CFG_FLOAT_KEY) != std::string::npos) {
				g_vars.set(key, std::stof(util::remove_chars_from_string(value, CFG_FLOAT_KEY)));
			}
		}
	}

	ifs.close();
}

void config::save(const std::string& name)
{
	std::ofstream ofs{ name, std::ios::binary };

	if (!ofs.is_open())
		return;

	std::ostringstream oss{};

	for (const auto&[k, v] : g_vars.get_map()) {
		oss << k << "=";
		std::visit([&](auto&& arg) {
			using T = std::decay_t<decltype(arg)>;

			if constexpr (std::is_same_v<T, bool>)
				oss << (arg ? CFG_BOOL_TRUE_KEY : CFG_BOOL_FALSE_KEY);

			else if constexpr (std::is_same_v<T, int>)
				oss << CFG_INT_KEY << arg;

			else if constexpr (std::is_same_v<T, float>)
				oss << CFG_FLOAT_KEY << arg;

			oss << std::endl;
		}, v);
	}

	std::string encrypted{ util::xor_encrypt_decrypt(oss.str(), name) };

	ofs.write(encrypted.c_str(), encrypted.size());
	ofs.close();
}