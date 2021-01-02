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
extern Model g_model;
/*
extern Queue<State, 32> g_queue_mainToAudio;
extern Queue<State, 32> g_queue_audioToMain;
extern State            g_state; // main thread state
*/
namespace api
{
namespace
{
float pitchOld_ = 0.0;
} // {anonymous}


/* -------------------------------------------------------------------------- */
/* -------------------------------------------------------------------------- */
/* -------------------------------------------------------------------------- */


void play()
{
    g_model.requestChange([]()
    {
        g_model.state.status = ReadStatus::PLAY;
    });
}


/* -------------------------------------------------------------------------- */


void stop()
{
    g_model.requestChange([]()
    {
        g_model.state.status = ReadStatus::STOP;
    });
}


/* -------------------------------------------------------------------------- */


void playPauseToggle()
{
    g_model.requestChange([]()
    {
        g_model.state.status = g_model.state.status == ReadStatus::PLAY ? ReadStatus::PAUSE : ReadStatus::PLAY; 
    });
}


/* -------------------------------------------------------------------------- */


void rewind()
{
    g_model.requestChange([]()
    {
        g_model.state.position = 0;
    });
}


/* -------------------------------------------------------------------------- */


bool loadAudioFile(std::string path)
{
    std::shared_ptr<AudioFile> audioFile = engine::makeAudioFile(path, 44100);
    if (audioFile == nullptr)
        return false;

    g_model.requestChange([audioFile = std::move(audioFile)]()
    {
        g_model.layout.audioFile = audioFile;
    });

    return true;
}


/* -------------------------------------------------------------------------- */


void setPitch(PitchDir dir)
{
    g_model.requestChange([dir]()
    {
        g_model.layout.pitch += dir == PitchDir::UP ? G_PITCH_DELTA : -G_PITCH_DELTA;
    });
}


void nudgePitch_begin(PitchDir dir)
{
    /*
    g_model.queueIn.push([dir]()
    {
        g_model.state.pitch += dir == PitchDir::UP ? G_PITCH_DELTA : -G_PITCH_DELTA;
    });

    onPushState_([dir] (State& s) 
    { 
        pitchOld_ = s.pitch;
        s.pitch = pitchOld_ + (dir == PitchDir::UP ? G_PITCH_NUDGE : -G_PITCH_NUDGE);
    });*/
}


void nudgePitch_end()
{
    /*assert(pitchOld_ != 0.0); // Must follow a pitchNudge_begin call

    onPushState_([] (State& s) { s.pitch = pitchOld_; });
    pitchOld_ = 0.0;*/
}


/* -------------------------------------------------------------------------- */


State getCurrentState()
{
    return g_model.requestState();
}
} // geena::engine::
} // geena::engine::api::