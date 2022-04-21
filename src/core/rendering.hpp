#pragma once

#include "audioFile.hpp"
#include "deps/audio-buffer/src/audioBuffer.hpp"
#include "deps/mcl-resampler/src/resampler.hpp"
#include "types.hpp"

namespace geena::core
{
class Renderer
{
public:
	Renderer();

	/* render
	Renders a portion of AudioFile to the AudioBuffer, starting from frame 
	'position'. */

	Frame render(const AudioFile&, mcl::AudioBuffer&, float pitch, Frame position);

private:
	Frame renderResampled(const mcl::AudioBuffer& in, mcl::AudioBuffer& out, float pitch, Frame position);

	mcl::Resampler m_resampler;
};
} // namespace geena::core