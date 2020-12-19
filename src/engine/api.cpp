#include <cassert>
#include "utils/log.hpp"
#include "state.hpp"
#include "const.hpp"
#include "queue.hpp"
#include "event.hpp"
#include "circular.hpp"
#include "audioFileFactory.hpp"
#include "api.hpp"


namespace geena::engine
{
extern Circular<State>  g_state;
extern Queue<Event, 32> g_queue;
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
    g_queue.push({EventType::PLAY});
}


/* -------------------------------------------------------------------------- */


void stop()
{
    g_queue.push({EventType::STOP});
    /*
    g_queue.push({EventType::STOP});
    while (g_state.isLocked())
    {
        G_DEBUG("Wait for render to finish block...");
    };
    G_DEBUG("Block finished");*/
}


/* -------------------------------------------------------------------------- */


bool loadAudioFile(std::string path)
{
    std::shared_ptr<AudioFile> audioFile = engine::makeAudioFile(path, 44100);
    if (audioFile == nullptr)
        return false;
    g_queue.push({EventType::SET_AUDIO_FILE, {}, audioFile});
    return true;
}


/* -------------------------------------------------------------------------- */


void setPitch(PitchDir dir)
{
    float pitch = g_state.load().pitch + (dir == PitchDir::UP ? G_PITCH_DELTA : -G_PITCH_DELTA);
    g_queue.push({EventType::SET_PITCH, pitch});
}


void nudgePitch_begin(PitchDir dir)
{
    pitchOld_ = g_state.load().pitch;
    float pitch = pitchOld_ + (dir == PitchDir::UP ? G_PITCH_NUDGE : -G_PITCH_NUDGE);
    g_queue.push({EventType::SET_PITCH, pitch});
}


void nudgePitch_end()
{
    assert(pitchOld_ != 0.0); // Must follow a pitchNudge_begin call

    g_queue.push({EventType::SET_PITCH, pitchOld_});
    pitchOld_ = 0.0;
}
} // geena::engine::
} // geena::engine::api::