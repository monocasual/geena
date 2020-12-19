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

    const AudioFile* getAudioFile() const;

    bool isLocked() const;
    
    void lock();
    void unlock();
    void setAudioFile(AudioFile&&);
    
    std::atomic<bool>   rendering;
    std::atomic<Status> status;
    std::atomic<Frame>  position;
    std::atomic<float>  pitch;
    
private:

    std::atomic<bool> m_lock;
    AudioFile         m_audioFile;
};
} // geena::engine::