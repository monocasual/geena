#include <cassert>
#include "utils/log.hpp"
#include "state.hpp"
#include "const.hpp"
#include "audioFileFactory.hpp"
#include "api.hpp"


extern geena::engine::State g_state;


namespace geena::engine::api
{
namespace
{
float pitchOld_ = 0.0;
} // {anonymous}


/* -------------------------------------------------------------------------- */
/* -------------------------------------------------------------------------- */
/* -------------------------------------------------------------------------- */


void startRendering()
{
    g_state.rendering.store(true);
}


/* -------------------------------------------------------------------------- */


void stopRendering()
{
    g_state.rendering.store(false);
    while (g_state.isLocked())
    {
        G_DEBUG("Wait for render to finish block...");
    };
    G_DEBUG("Block finished");
}


/* -------------------------------------------------------------------------- */


bool loadAudioFile(std::string path)
{
    stopRendering();

    std::optional<AudioFile> audioFile = engine::makeAudioFile(path, 44100);
    if (!audioFile)
        return false;

    g_state.setAudioFile(std::move(audioFile.value()));

    startRendering();
    return true;
}


/* -------------------------------------------------------------------------- */


void setPitch(PitchDir dir)
{
    float pitch = g_state.pitch.load();
    pitch += dir == PitchDir::UP ? G_PITCH_DELTA : -G_PITCH_DELTA;
    g_state.pitch.store(pitch);
}


void nudgePitch_begin(PitchDir dir)
{
    pitchOld_ = g_state.pitch.load();
    float pitch = pitchOld_ + (dir == PitchDir::UP ? G_PITCH_NUDGE : -G_PITCH_NUDGE);
    g_state.pitch.store(pitch);
}


void nudgePitch_end()
{
    assert(pitchOld_ != 0.0); // Must follow a pitchNudgeBegin call

    g_state.pitch.store(pitchOld_);
    pitchOld_ = 0.0;
}
} // geena::engine::api::