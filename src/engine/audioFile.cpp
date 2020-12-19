#include <sndfile.h>
#include "audioFile.hpp"


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
	b.copyData(m_data[start], count, /*CHANNEL TODO*/2, offset);
}
} // geena::engine::