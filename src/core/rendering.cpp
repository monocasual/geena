#include "rendering.hpp"
#include <cmath>

using namespace mcl;

namespace geena::core
{
Renderer::Renderer()
: m_resampler(Resampler::Quality::LINEAR, /*TODO channels=*/2)
{
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
	float* input        = in[0];
	long   inputPos     = position;
	long   inputLength  = in.countFrames();
	float* output       = out[0];
	long   outputLength = out.countFrames();
	float  ratio        = pitch;

	Resampler::Result res = m_resampler.process(input, inputPos, inputLength, output,
	    outputLength, ratio);

	//if (res.used < out.countFrames())
	//	m_resampler.last();

	return res.used;
}
} // namespace geena::core