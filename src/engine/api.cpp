#include <cassert>
#include <functional>
#include "utils/log.hpp"
#include "state.hpp"
#include "const.hpp"
#include "queue.hpp"
#include "audioFileFactory.hpp"
#include "api.hpp"


namespace geena::engine::api
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
    engine::getState().status.store(ReadStatus::PLAY);
}


/* -------------------------------------------------------------------------- */


void pause()
{
    engine::getState().status.store(ReadStatus::PAUSE);
}


/* -------------------------------------------------------------------------- */


void playPauseToggle()
{
    engine::getState().status.load() == ReadStatus::PLAY ? pause() : play();
}


/* -------------------------------------------------------------------------- */


void rewind()
{
    engine::getState().position.store(0);
}


/* -------------------------------------------------------------------------- */


bool loadAudioFile(std::string path)
{
    auto res = engine::makeAudioFile(path, 44100);
    if (!res)
        return false;
    
    engine::getData().audioFile = std::move(res.value());

	onSwapLayout([](Layout& layout)
	{
		layout.audioFile = &engine::getData().audioFile;
	});

    play();

    return true;
}


void unloadAudioFile()
{
	/* Layout first, then data. */
	onSwapLayout([](Layout& layout)
	{
		layout.audioFile = nullptr;
	});	

	engine::getData().audioFile = {};
	rewind();
}


/* -------------------------------------------------------------------------- */


void setPitch(PitchDir dir)
{
	onSwapLayout([dir](Layout& layout)
	{
		layout.pitch += dir == PitchDir::UP ? G_PITCH_DELTA : -G_PITCH_DELTA;
	});
}


void nudgePitch_begin(PitchDir dir)
{
	onSwapLayout([dir](Layout& layout)
	{
		pitchOld_ = layout.pitch;
		layout.pitch = pitchOld_ + (dir == PitchDir::UP ? G_PITCH_NUDGE : -G_PITCH_NUDGE);
	});
}


void nudgePitch_end()
{
    assert(pitchOld_ != 0.0); // Must follow a pitchNudge_begin call
   
	onSwapLayout([](Layout& layout)
	{
		layout.pitch = pitchOld_;
	});

    pitchOld_ = 0.0;
}


/* -------------------------------------------------------------------------- */


Frame getCurrentPosition()
{
    return engine::getState().position.load();
}

Frame getAudioFileLength()
{
    const AudioFile& f = engine::getData().audioFile;
    return f.isValid() ? f.countFrames() : 0;
}

} // geena::engine::api::