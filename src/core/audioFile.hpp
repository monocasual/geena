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

	bool                    isValid() const;
	Frame                   countFrames() const;
	const mcl::AudioBuffer& getBuffer() const;

	/* render
	Fills AudioBuffer with its own audio data, reading audio data from frame
	'start'. Returns how many frames have been used. */

	Frame render(mcl::AudioBuffer&, Frame start) const;

private:
	mcl::AudioBuffer m_data;
};
} // namespace geena::core