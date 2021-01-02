#pragma once


#include <memory>
#include <functional>
#include "queue.hpp"
#include "types.hpp"
#include "audioFile.hpp"


namespace geena::engine
{
struct Layout
{
    std::shared_ptr<AudioFile> audioFile;
    float                      pitch = 1.0f;
};

struct State
{
    bool       valid    = false;
    ReadStatus status   = ReadStatus::STOP;
    Frame      position = 0;
};

struct Buffers
{
    AudioBuffer audioBuffer;
};

struct Model
{
    Layout  layout;  // NEVER access this from the main thread directly
    State   state;   // NEVER access this from the main thread directly
    Buffers buffers; // NEVER access this from the main thread directly

    /* Called by **Audio thread**, applies all the pending changes previously
    required by the main thread. */

    void applyChanges();

    /* Called by **Main thread**, requires to apply the function f to the current
    model. Will be performed later on by Audio thread. No allocation, no 
    exception throwing, always move! */

    void requestChange(std::function<void()>&& f);

    /* Called by **Main thread**, asks a copy of the whole layout. Will be copied
    and returned by the Audio thread later on. */

    Layout requestLayout();

    /* Called by **Main thread**, asks a copy of the state for UI refresh. Will
    be copied and returned by the Audio thread later on. */

    State requestState();

private:

    Queue<std::function<void()>, 32> m_queueIn;
    Queue<Layout, 32>                m_queueOutLayout;
    Queue<State, 32>                 m_queueOutState;
};
} // geena::engine::