#include "audioFile.hpp"
#include <sndfile.h>

using namespace mcl;

namespace geena::core
{
AudioFile::AudioFile()
{
}

/* -------------------------------------------------------------------------- */

AudioFile::AudioFile(AudioBuffer&& data, const std::string& path)
: m_data(std::move(data))
, m_path(path)
{
}

/* -------------------------------------------------------------------------- */

bool AudioFile::isValid() const { return m_data.isAllocd(); }

/* -------------------------------------------------------------------------- */

Frame AudioFile::countFrames() const { return m_data.countFrames(); }
Frame AudioFile::countChannels() const { return m_data.countChannels(); }

/* -------------------------------------------------------------------------- */

const AudioBuffer& AudioFile::getBuffer() const { return m_data; }

/* -------------------------------------------------------------------------- */

std::string AudioFile::getPath() const { return m_path; }

/* -------------------------------------------------------------------------- */

Frame AudioFile::render(AudioBuffer& b, Frame start) const
{
	const Frame amount = std::min(b.countFrames(), m_data.countFrames() - start);
	b.set(m_data, amount, start);
	return amount;
}
} // namespace geena::core