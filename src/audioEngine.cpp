#include <cassert>
#include "deps/rtaudio/RtAudio.h"
#include "audioEngine.hpp"


namespace geena::m::audioEngine
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
	callback_(out, in, bufferSize); 
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
		rt_.openStream(&params, NULL, RTAUDIO_FLOAT32, c.sampleRate, &bufferSize_, &rtCallback_);
		rt_.startStream();
		callback_ = f;
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

} // geena::m::