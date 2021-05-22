#pragma once

#include "deps/audio-buffer/src/audioBuffer.hpp"
#include "types.hpp"
#include <memory>
#include <string>

namespace geena::engine
{
class AudioFile
{
public:
	AudioFile();
	AudioFile(monocasual::AudioBuffer&& data);

	bool                           isValid() const;
	Frame                          countFrames() const;
	void                           render(monocasual::AudioBuffer& b, Frame start, Frame count, Frame offset = 0) const;
	const monocasual::AudioBuffer& getBuffer() const;

private:
	monocasual::AudioBuffer m_data;
};
} // namespace geena::engine