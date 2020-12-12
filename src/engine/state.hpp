#pragma once


#include <atomic>
#include "types.hpp"
#include "audioFile.hpp"


namespace geena::engine
{
class State
{
public:

    State(); 

    const AudioFile& getAudioFile() const;

    void lock();
    void unlock();
    bool tryLock();
    void setAudioFile(AudioFile&&);
    
    std::atomic<Status> status;
    std::atomic<Frame>  position;
    std::atomic<float>  pitch;
    
private:

    std::atomic<bool> m_lock;
    AudioFile         m_audioFile;
};
} // geena::engine::