#include "audioFile.hpp"
#include <sndfile.h>

using namespace monocasual;

namespace geena::engine
{
AudioFile::AudioFile()
{
}

/* -------------------------------------------------------------------------- */

AudioFile::AudioFile(AudioBuffer&& data)
: m_data(std::move(data))
{
}

/* -------------------------------------------------------------------------- */

bool AudioFile::isValid() const { return m_data.isAllocd(); }

/* -------------------------------------------------------------------------- */

Frame AudioFile::countFrames() const { return m_data.countFrames(); }

/* -------------------------------------------------------------------------- */

const AudioBuffer& AudioFile::getBuffer() const { return m_data; }

/* -------------------------------------------------------------------------- */

void AudioFile::render(AudioBuffer& b, Frame start, Frame count, Frame offset) const
{
	// TODO b.copyData(m_data[start], count, /*CHANNEL TODO*/2, offset);
	b.set(m_data, count, start);
}
} // namespace geena::engine