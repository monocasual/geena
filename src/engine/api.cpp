#include <cassert>
#include <functional>
#include "utils/log.hpp"
#include "state.hpp"
#include "const.hpp"
#include "queue.hpp"
#include "audioFileFactory.hpp"
#include "api.hpp"


namespace geena::engine
{
extern Queue<State, 32> g_queue_mainToAudio;
extern Queue<State, 32> g_queue_audioToMain;
extern State            g_state; // main thread state
namespace api
{
namespace
{
float pitchOld_ = 0.0;


/* -------------------------------------------------------------------------- */


void onPushState_(std::function<void(State&)> f)
{
    refreshMainState();   // Get the most up-to-date state from the audio thread
    State state = g_state;
    f(state);
    g_queue_mainToAudio.push(state);
    g_queue_audioToMain.push(state);
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


void refreshMainState()
{
	while (auto o = g_queue_audioToMain.pop())
    {
		g_state = o.value();
        G_DEBUG("Pop new State from [AUDIO] thread");
		G_DEBUG("  status=" << (int) g_state.status);
		G_DEBUG("  position=" << g_state.position);
		G_DEBUG("  pitch=" << g_state.pitch);
		G_DEBUG("  audioFile=" << (g_state.audioFile != nullptr));
    }
}


const State& getState()
{
    return g_state;
}
} // geena::engine::
} // geena::engine::api::