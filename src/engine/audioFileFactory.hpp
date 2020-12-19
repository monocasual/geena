#pragma once


#include <string>
#include <optional>
#include "audioFile.hpp"


namespace geena::engine
{
std::shared_ptr<AudioFile> makeAudioFile(std::string path, int sampleRate);
} // geena::engine::