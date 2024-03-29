#pragma once

#include "core/config.hpp"
#include "deps/audio-buffer/src/audioBuffer.hpp"
#include <RtAudio.h>
#include <functional>

namespace geena::core
{
class Kernel
{
public:
	using Callback = std::function<void(mcl::AudioBuffer&)>;

	Config getConfig() const;

	bool init(Config c, Callback f);
	void close();

private:
	static int callback(void*, void*, unsigned, double, RtAudioStreamStatus, void*);

	RtAudio  m_rt;
	Callback m_callback = nullptr;
	Config   m_config;
};
} // namespace geena::core