#pragma once


#include <memory>
#include <functional>
#include "queue.hpp"
#include "types.hpp"
#include "audioFile.hpp"
#include "swapper.hpp"


namespace geena::engine
{
struct State
{
    std::atomic<ReadStatus> status{ReadStatus::STOP};
    std::atomic<Frame>      position{0};
};

struct Layout
{
    float      pitch     = 1.0f;
    AudioFile* audioFile = nullptr;
    State*     state     = nullptr;
};

struct Data
{
    AudioFile audioFile;
};
} // geena::engine::