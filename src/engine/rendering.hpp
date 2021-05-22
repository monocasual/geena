#pragma once

#include "audioFile.hpp"
#include "deps/audio-buffer/src/audioBuffer.hpp"
#include "types.hpp"

namespace geena::engine::renderer
{
void  init();
Frame render(const AudioFile& f, monocasual::AudioBuffer& out, float pitch, Frame position, Frame bufferSize);
} // namespace geena::engine::renderer