#include "core/rendering.hpp"
#include "deps/mcl-utils/src/log.hpp"
#include <cassert>
#include <cmath>

using namespace mcl;

namespace geena::core
{
void Renderer::init(Frame systemBufferSize, int numAudioFileChannels)
{
	m_resampler = {Resampler::Quality::LINEAR, numAudioFileChannels};
	m_buffer    = {systemBufferSize, numAudioFileChannels};

	ML_DEBUG("Renderer initialized - bufSize=" << systemBufferSize << " chans=" << numAudioFileChannels);
}

/* -------------------------------------------------------------------------- */

Frame Renderer::render(const AudioFile& file, AudioBuffer& out, float pitch, Frame position)
{
	Frame used = 0;

	if (pitch == 1.0)
		used = file.render(out, position);
	else
		used = renderResampled(file.getBuffer(), out, pitch, position);

	return position + used;
}

/* -------------------------------------------------------------------------- */

Frame Renderer::renderResampled(const AudioBuffer& in, AudioBuffer& out, float pitch, Frame position)
{
	assert(in.countChannels() <= 2);
	assert(in.countChannels() == m_buffer.countChannels());
	assert(out.countChannels() == 2);

	float* input        = in[0];
	long   inputPos     = position;
	long   inputLength  = in.countFrames();
	float* output       = out[0];
	long   outputLength = out.countFrames();
	bool   localRender  = in.countChannels() != out.countChannels();

	/* Output Resampler's result to temporary buffer, which matches the number
	of channels of the input buffer. */

	if (localRender)
	{
		output       = m_buffer[0];
		outputLength = m_buffer.countFrames();
	}

	Resampler::Result res = m_resampler.process(input, inputPos, inputLength, output,
	    outputLength, pitch);

	if (localRender)
		out.set(m_buffer, 1.0f);

	return res.used;
}
} // namespace geena::core