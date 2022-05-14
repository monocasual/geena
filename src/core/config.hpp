#pragma once

#include <optional>
#include <string>

namespace geena::core
{
struct Config
{
	int      deviceId;
	int      channels;
	int      sampleRate;
	unsigned bufferSize;
};
} // namespace geena::core

namespace geena::core::config
{
std::optional<Config> read(const std::string& path);
}
