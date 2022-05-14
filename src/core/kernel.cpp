#include "kernel.hpp"
#include "deps/mcl-utils/src/log.hpp"
#include <cassert>

using namespace mcl;

namespace geena::core
{
bool Kernel::init(Config c, Callback f)
{
	m_config   = c;
	m_callback = f;

	RtAudio::StreamParameters params;
	params.deviceId     = m_config.deviceId;
	params.nChannels    = m_config.channels;
	params.firstChannel = 0;

	try
	{
		m_rt.openStream(&params, nullptr, RTAUDIO_FLOAT32, m_config.sampleRate, &m_config.bufferSize, &callback, (void*)this);
		m_rt.startStream();
		return true;
	}
	catch (RtAudioError& e)
	{
		ML_DEBUG("Open stream error: " << e.getMessage());
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
/* -------------------------------------------------------------------------- */

Config Kernel::getConfig() const
{
	return m_config;
}
} // namespace geena::core