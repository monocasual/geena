#pragma once


#include <memory>
#include "types.hpp"
#include "audioFile.hpp"


namespace geena::engine
{
struct State
{
    State(); 

    Status status;
    Frame  position;
    float  pitch;
    std::shared_ptr<AudioFile> audioFile;
};
} // geena::engine::