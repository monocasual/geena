#pragma once


#include <memory>
#include <functional>
#include "queue.hpp"
#include "types.hpp"
#include "audioFile.hpp"


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

const Layout& rt_lock();
void          rt_unlock();

Layout& getLayout();
State&  getState();
Data&   getData();
void    onSwapLayout(std::function<void(Layout&)> f);
} // geena::engine::