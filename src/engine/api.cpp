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


/* -------------------------------------------------------------------------- */


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
    onPushState_([] (State& s) { s.status = ReadStatus::PLAY; });
}


/* -------------------------------------------------------------------------- */


void stop()
{
    onPushState_([] (State& s) { s.status = ReadStatus::STOP; });
}


/* -------------------------------------------------------------------------- */


void playPauseToggle()
{
    onPushState_([] (State& s) 
    { 
        s.status = s.status == ReadStatus::PLAY ? ReadStatus::PAUSE : ReadStatus::PLAY; 
    });
}


/* -------------------------------------------------------------------------- */


void rewind()
{
    onPushState_([] (State& s) { s.position = 0; });
}


/* -------------------------------------------------------------------------- */


bool loadAudioFile(std::string path)
{
    std::shared_ptr<AudioFile> audioFile = engine::makeAudioFile(path, 44100);
    if (audioFile == nullptr)
        return false;
    
    onPushState_([&audioFile] (State& s) { s.audioFile = audioFile; });
    return true;
}


/* -------------------------------------------------------------------------- */


void setPitch(PitchDir dir)
{
    onPushState_([dir] (State& s) 
    { 
        s.pitch += dir == PitchDir::UP ? G_PITCH_DELTA : -G_PITCH_DELTA;
    });
}


void nudgePitch_begin(PitchDir dir)
{
    onPushState_([dir] (State& s) 
    { 
        pitchOld_ = s.pitch;
        s.pitch = pitchOld_ + (dir == PitchDir::UP ? G_PITCH_NUDGE : -G_PITCH_NUDGE);
    });
}


void nudgePitch_end()
{
    assert(pitchOld_ != 0.0); // Must follow a pitchNudge_begin call

    onPushState_([] (State& s) { s.pitch = pitchOld_; });
    pitchOld_ = 0.0;
}


/* -------------------------------------------------------------------------- */


State getState()
{
    return g_state.load();
}
} // geena::engine::
} // geena::engine::api::