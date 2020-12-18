#include <cassert>
#include <RtAudio.h>
#include "audioEngine.hpp"


namespace geena::engine::kernel
{
namespace
{
RtAudio  rt_;
Callback callback_   = nullptr;
unsigned bufferSize_ = 0;


/* -------------------------------------------------------------------------- */


int rtCallback_(void* out, void* in, unsigned bufferSize, double streamTime, 
	          RtAudioStreamStatus status, void *userData)
{
	assert(callback_ != nullptr);

	AudioBuffer bufferOut, bufferIn;
	bufferOut.setData(static_cast<float*>(out), bufferSize, /*G_MAX_IO_CHANS TODO */2);
	bufferIn.setData(static_cast<float*>(in), bufferSize, /*G_MAX_IO_CHANS TODO */2);

	bufferOut.clear();

	callback_(bufferOut, bufferIn, bufferSize);

	/* Unset data in buffers. If you don't do this, buffers go out of scope and 
	destroy memory allocated by RtAudio ---> havoc. */

	bufferOut.setData(nullptr, 0, 0);
	bufferIn.setData(nullptr, 0, 0);

	return 0;
}
} // {anonymous}


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
} // geena::engine::kernel::