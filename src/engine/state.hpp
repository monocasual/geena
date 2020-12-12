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
    
    std::atomic<Status> status;
    std::atomic<Frame>  position;
    std::atomic<float>  pitch;
    AudioFile           audioFile;
};
} // geena::engine::