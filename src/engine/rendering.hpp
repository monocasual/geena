#pragma once


#include "types.hpp"
#include "audioFile.hpp"
#include "deps/audio-buffer/src/audioBuffer.hpp"

using namespace monocasual;


namespace geena::engine::renderer
{
void init();
Frame render(const AudioFile& f, AudioBuffer& out, float pitch, Frame position, Frame bufferSize);
} // geena::engine::renderer::