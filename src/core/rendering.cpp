#include "rendering.hpp"
#include <cmath>

using namespace mcl;

namespace geena::core
{
void Renderer::init()
{
	m_srcState = src_new(SRC_LINEAR, 2 /*CHANS TODO*/, nullptr);
	if (m_srcState == nullptr)
		throw std::bad_alloc();

	// TODO - delete!
}

/* -------------------------------------------------------------------------- */

Frame Renderer::render(const AudioFile& file, AudioBuffer& out, float pitch, Frame position, Frame bufferSize)
{
	Frame framesUsed = 0;

	if (pitch == 1.0)
	{
		file.render(out, position, bufferSize);
		framesUsed = position + bufferSize;
	}
	else
	{
		framesUsed = renderResampled(file.getBuffer(), out, pitch, position);
	}

	return framesUsed;
}

/* -------------------------------------------------------------------------- */

Frame Renderer::renderResampled(const AudioBuffer& in, AudioBuffer& out, float pitch, Frame position)
{
	SRC_DATA srcData;

	srcData.data_in       = in[position];                // Source data
	srcData.input_frames  = in.countFrames() - position; // How many readable frames
	srcData.data_out      = out[0];                      // Destination (processed data)
	srcData.output_frames = out.countFrames();           // How many frames to process
	srcData.end_of_input  = false;
	srcData.src_ratio     = 1.0 / pitch;

	src_process(m_srcState, &srcData);

	return position + srcData.input_frames_used;
}
} // namespace geena::engine