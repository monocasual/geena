#include <cassert>
#include "state.hpp"


namespace geena::engine
{
State::State()
: status  (Status::OFF)
, position(0)
, pitch   (0.8f)
{
}


/* -------------------------------------------------------------------------- */


const AudioFile& State::getAudioFile() const
{
    assert(m_lock.load(std::memory_order_acquire) == true);
    return m_audioFile;
}


void State::setAudioFile(AudioFile&& audioFile)
{
    assert(m_lock.load(std::memory_order_acquire) == true);
    m_audioFile = std::move(audioFile);
}



/* -------------------------------------------------------------------------- */


void State::lock()   { m_lock.store(true, std::memory_order_release); }
void State::unlock() { m_lock.store(false, std::memory_order_release); }


/* -------------------------------------------------------------------------- */


bool State::tryLock()
{
    /* std::atomic<T>::exchange returns the previous value: if it's 'true', a 
    lock is already in taken. */

    return m_lock.exchange(true, std::memory_order_acquire) == false;
}
} // geena::engine::