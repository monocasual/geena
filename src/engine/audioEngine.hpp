#pragma once

#include "deps/audio-buffer/src/audioBuffer.hpp"
#include <functional>

namespace geena::engine::kernel
{
using Callback = std::function<void(mcl::AudioBuffer&, unsigned)>;

struct Config
{
	int deviceId;
	int channels;
	int sampleRate;
	int bufferSize;
};

bool init(Config c, Callback f);
void close();
} // namespace geena::engine::kernel