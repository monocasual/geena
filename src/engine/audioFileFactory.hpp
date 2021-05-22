#pragma once


#include <string>
#include <optional>
#include "audioFile.hpp"

namespace geena::engine
{
std::optional<AudioFile> makeAudioFile(std::string path, int sampleRate);
} // geena::engine::