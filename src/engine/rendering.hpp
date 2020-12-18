#pragma once


#include "types.hpp"
#include "audioFile.hpp"
#include "audioBuffer.hpp"


namespace geena::engine::renderer
{
void init();
Frame render(const AudioFile& f, AudioBuffer& out, float pitch, Frame position, Frame bufferSize);
} // geena::engine::renderer::