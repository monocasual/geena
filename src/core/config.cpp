#include "config.hpp"
#include "deps/mcl-utils/src/log.hpp"
#include <fstream>
#include <nlohmann/json.hpp>

using namespace mcl;
using json = nlohmann::json;

namespace geena::core::config
{
std::optional<Config> read(const std::string& path)
{
	std::ifstream ifs(path);
	if (!ifs.good())
	{
		ML_DEBUG("Can't find configuration file, aborting");
		return {};
	}

	json j = json::parse(ifs);

	Config config;
	config.deviceId   = j["deviceId"];
	config.channels   = j["channels"];
	config.sampleRate = j["sampleRate"];
	config.bufferSize = j["bufferSize"];

	return {config};
}
} // namespace geena::core::config