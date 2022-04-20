#pragma once

#include "audioFile.hpp"
#include <optional>
#include <string>

namespace geena::core
{
std::optional<AudioFile> makeAudioFile(std::string path, int sampleRate);
} // namespace geena::engine