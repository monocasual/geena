#pragma once

#include "deps/audio-buffer/src/audioBuffer.hpp"
#include "types.hpp"
#include <memory>
#include <string>

namespace geena::core
{
class AudioFile
{
public:
	AudioFile();
	AudioFile(mcl::AudioBuffer&& data);

	bool                           isValid() const;
	Frame                          countFrames() const;
	void                           render(mcl::AudioBuffer& b, Frame start, Frame count, Frame offset = 0) const;
	const mcl::AudioBuffer& getBuffer() const;

private:
	mcl::AudioBuffer m_data;
};
} // namespace geena::engine