#include <cassert>
#include <functional>
#include "utils/log.hpp"
#include "state.hpp"
#include "const.hpp"
#include "queue.hpp"
#include "circular.hpp"
#include "audioFileFactory.hpp"
#include "api.hpp"


namespace geena::engine
{
extern Circular<State>  g_state;
extern Queue<State, 32> g_queue;
namespace api
{
namespace
{
float pitchOld_ = 0.0;

void onPushState_(std::function<void(State&)> f)
{
    State state = g_state.load();
    f(state);
    g_queue.push(state);
}
} // {anonymous}


/* -------------------------------------------------------------------------- */
/* -------------------------------------------------------------------------- */
/* -------------------------------------------------------------------------- */


void play()
{
    onPushState_([] (State& s) { s.status = Status::PLAY; });
}


/* -------------------------------------------------------------------------- */


void stop()
{
    onPushState_([] (State& s) { s.status = Status::STOP; });
}


/* -------------------------------------------------------------------------- */


bool loadAudioFile(std::string path)
{
    std::shared_ptr<AudioFile> audioFile = engine::makeAudioFile(path, 44100);
    if (audioFile == nullptr)
        return false;
    State state = g_state.load();
    state.audioFile = audioFile;
    state.status    = Status::PLAY;
    g_queue.push(state);
    return true;
}


/* -------------------------------------------------------------------------- */


void setPitch(PitchDir dir)
{
    State state = g_state.load();
    state.pitch += dir == PitchDir::UP ? G_PITCH_DELTA : -G_PITCH_DELTA;
    g_queue.push(state);
}


void nudgePitch_begin(PitchDir dir)
{
    State state = g_state.load();
    pitchOld_ = state.pitch;
    state.pitch = pitchOld_ + (dir == PitchDir::UP ? G_PITCH_NUDGE : -G_PITCH_NUDGE);
    g_queue.push(state);
}


void nudgePitch_end()
{
    assert(pitchOld_ != 0.0); // Must follow a pitchNudge_begin call

    State state = g_state.load();
    state.pitch = pitchOld_;
    pitchOld_ = 0.0;
    g_queue.push(state);
}
} // geena::engine::
} // geena::engine::api::