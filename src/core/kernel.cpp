#include "kernel.hpp"
#include <cassert>

using namespace mcl;

namespace geena::core
{
bool Kernel::init(Config c, Callback f)
{
	RtAudio::StreamParameters params;
	params.deviceId     = c.deviceId;
	params.nChannels    = c.channels;
	params.firstChannel = 0;

	m_bufferSize = c.bufferSize;
	m_callback   = f;

	try
	{
		m_rt.openStream(&params, nullptr, RTAUDIO_FLOAT32, c.sampleRate, &m_bufferSize, &callback, (void*)this);
		m_rt.startStream();
		return true;
	}
	catch (RtAudioError& e)
	{
		//e.printMessage();
		return false;
	}
}

/* -------------------------------------------------------------------------- */

void Kernel::close()
{
	try
	{
		m_rt.stopStream();
	}
	catch (RtAudioError& e)
	{
		e.printMessage();
	}
	if (m_rt.isStreamOpen())
		m_rt.closeStream();
}

/* -------------------------------------------------------------------------- */

int Kernel::callback(void* out, void* /*in*/, unsigned bufferSize, double /*streamTime*/,
    RtAudioStreamStatus /*status*/, void* data)
{
	Kernel* kernel = static_cast<Kernel*>(data);

	AudioBuffer bufferOut(static_cast<float*>(out), bufferSize, /*G_MAX_IO_CHANS TODO */ 2);
	bufferOut.clear();

	kernel->m_callback(bufferOut);

	return 0;
}
} // namespace geena::core