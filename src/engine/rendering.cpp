#include "rendering.hpp"
#include <cmath>
#include <samplerate.h>

using namespace monocasual;

namespace geena::engine::renderer
{
namespace
{
SRC_STATE* srcState_;

/* -------------------------------------------------------------------------- */

Frame renderResampled(const AudioBuffer& in, AudioBuffer& out, float pitch, Frame position)
{
	SRC_DATA srcData;

	srcData.data_in       = in[position];                // Source data
	srcData.input_frames  = in.countFrames() - position; // How many readable frames
	srcData.data_out      = out[0];                      // Destination (processed data)
	srcData.output_frames = out.countFrames();           // How many frames to process
	srcData.end_of_input  = false;
	srcData.src_ratio     = 1.0 / pitch;

	src_process(srcState_, &srcData);

	return position + srcData.input_frames_used;
}
} // namespace

/* -------------------------------------------------------------------------- */
/* -------------------------------------------------------------------------- */
/* -------------------------------------------------------------------------- */

void init()
{
	srcState_ = src_new(SRC_LINEAR, 2 /*CHANS TODO*/, nullptr);
	if (srcState_ == nullptr)
		throw std::bad_alloc();

	// TODO - delete!
}

/* -------------------------------------------------------------------------- */

Frame render(const AudioFile& file, AudioBuffer& out, float pitch, Frame position, Frame bufferSize)
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
} // namespace geena::engine::renderer