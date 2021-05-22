#pragma once


#include <functional>
#include "deps/audio-buffer/src/audioBuffer.hpp"

using namespace monocasual;

namespace geena::engine::kernel
{
using Callback = std::function<void(AudioBuffer&, unsigned)>;

struct Config
{
    int deviceId;
    int channels;
    int sampleRate;
    int bufferSize;
};

bool init(Config c, Callback f);
void close();
} // geena::engine::kernel::