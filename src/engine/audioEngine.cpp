#include "audioEngine.hpp"
#include <RtAudio.h>
#include <cassert>

using namespace monocasual;

namespace geena::engine::kernel
{
namespace
{
RtAudio  rt_;
Callback callback_   = nullptr;
unsigned bufferSize_ = 0;

/* -------------------------------------------------------------------------- */

int rtCallback_(void* out, void* /*in*/, unsigned bufferSize, double /*streamTime*/,
    RtAudioStreamStatus /*status*/, void* /*userData*/)
{
	assert(callback_ != nullptr);

	AudioBuffer bufferOut(static_cast<float*>(out), bufferSize, /*G_MAX_IO_CHANS TODO */ 2);
	bufferOut.clear();

	callback_(bufferOut, bufferSize);

	return 0;
}
} // namespace

/* -------------------------------------------------------------------------- */
/* -------------------------------------------------------------------------- */
/* -------------------------------------------------------------------------- */

bool init(Config c, Callback f)
{
	RtAudio::StreamParameters params;
	params.deviceId     = rt_.getDefaultOutputDevice(); // TODO
	params.nChannels    = c.channels;
	params.firstChannel = 0;

	bufferSize_ = c.bufferSize;

	try
	{
		callback_ = f;
		rt_.openStream(&params, NULL, RTAUDIO_FLOAT32, c.sampleRate, &bufferSize_, &rtCallback_);
		rt_.startStream();
		return true;
	}
	catch (RtAudioError& e)
	{
		//e.printMessage();
		return false;
	}
}

/* -------------------------------------------------------------------------- */

void close()
{
	try
	{
		rt_.stopStream();
	}
	catch (RtAudioError& e)
	{
		e.printMessage();
	}
	if (rt_.isStreamOpen())
		rt_.closeStream();
}
} // namespace geena::engine::kernel