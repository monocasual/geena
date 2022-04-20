#pragma once

#include "audioFile.hpp"
#include "deps/audio-buffer/src/audioBuffer.hpp"
#include "types.hpp"
#include <samplerate.h>

namespace geena::core
{
class Renderer
{
public:
	void  init();
	Frame render(const AudioFile& f, mcl::AudioBuffer& out, float pitch, Frame position, Frame bufferSize);

private:
	Frame renderResampled(const mcl::AudioBuffer& in, mcl::AudioBuffer& out, float pitch, Frame position);

	SRC_STATE* m_srcState;
};
} // namespace geena::engine