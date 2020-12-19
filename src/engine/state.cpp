#include <cassert>
#include "state.hpp"


namespace geena::engine
{
State::State()
: rendering(false)
, status   (Status::OFF)
, position (0)
, pitch    (1.0f)
, m_lock   (false)
{
}


/* -------------------------------------------------------------------------- */


const AudioFile* State::getAudioFile() const
{
    return m_audioFile.isValid() ? &m_audioFile : nullptr;
}


void State::setAudioFile(AudioFile&& audioFile)
{
    assert(rendering.load() == false);
    m_audioFile = std::move(audioFile);
}



/* -------------------------------------------------------------------------- */


void State::lock()   { m_lock.store(true, std::memory_order_release); }
void State::unlock() { m_lock.store(false, std::memory_order_release); }


/* -------------------------------------------------------------------------- */

bool State::isLocked() const
{
    return m_lock.load(std::memory_order_acquire) == true;
}
} // geena::engine::