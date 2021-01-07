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
extern State           g_state;
extern Data            g_data;
extern Swapper<Layout> g_layout;	
}


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
    g_state.status.store(ReadStatus::PLAY);
}


/* -------------------------------------------------------------------------- */


void pause()
{
    g_state.status.store(ReadStatus::PAUSE);
}


/* -------------------------------------------------------------------------- */


void playPauseToggle()
{
    g_state.status.load() == ReadStatus::PLAY ? pause() : play();
}


/* -------------------------------------------------------------------------- */


void rewind()
{
    g_state.position.store(0);
}


/* -------------------------------------------------------------------------- */


bool loadAudioFile(std::string path)
{
    auto res = engine::makeAudioFile(path, 44100);
    if (!res)
        return false;
    
    g_data.audioFile = std::move(res.value());

	g_layout.onSwap([](Layout& layout)
	{
		layout.audioFile = &g_data.audioFile;
	});

    play();

    return true;
}


void unloadAudioFile()
{
	/* Layout first, then data. */
	g_layout.onSwap([](Layout& layout)
	{
		layout.audioFile = nullptr;
	});	

	g_data.audioFile = {};
	rewind();
}


/* -------------------------------------------------------------------------- */


void setPitch(PitchDir dir)
{
	g_layout.onSwap([dir](Layout& layout)
	{
		layout.pitch += dir == PitchDir::UP ? G_PITCH_DELTA : -G_PITCH_DELTA;
	});
}


void nudgePitch_begin(PitchDir dir)
{
	g_layout.onSwap([dir](Layout& layout)
	{
		pitchOld_ = layout.pitch;
		layout.pitch = pitchOld_ + (dir == PitchDir::UP ? G_PITCH_NUDGE : -G_PITCH_NUDGE);
	});
}


void nudgePitch_end()
{
    assert(pitchOld_ != 0.0); // Must follow a pitchNudge_begin call
   
	g_layout.onSwap([](Layout& layout)
	{
		layout.pitch = pitchOld_;
	});

    pitchOld_ = 0.0;
}


/* -------------------------------------------------------------------------- */


void goToFrame(Frame f)
{
	g_state.position.store(f);
}


/* -------------------------------------------------------------------------- */


Frame getCurrentPosition()
{
    return g_state.position.load();
}

Frame getAudioFileLength()
{
    const AudioFile& f = g_data.audioFile;
    return f.isValid() ? f.countFrames() : 0;
}

} // geena::engine::api::