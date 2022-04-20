#include "api.hpp"
#include "audioFileFactory.hpp"
#include "const.hpp"
#include "deps/atomic-swapper/src/atomic-swapper.hpp"
#include "engine.hpp"
#include "state.hpp"
#include <cassert>
#include <functional>

using namespace mcl;

namespace geena::core
{
extern Engine g_engine;
} // namespace geena::core

namespace geena::core::api
{
namespace
{
float pitchOld_ = 0.0;
} // namespace

/* -------------------------------------------------------------------------- */
/* -------------------------------------------------------------------------- */
/* -------------------------------------------------------------------------- */

void play()
{
	g_engine.state.status.store(ReadStatus::PLAY);
}

/* -------------------------------------------------------------------------- */

void pause()
{
	g_engine.state.status.store(ReadStatus::PAUSE);
}

/* -------------------------------------------------------------------------- */

void playPauseToggle()
{
	g_engine.state.status.load() == ReadStatus::PLAY ? pause() : play();
}

/* -------------------------------------------------------------------------- */

void rewind()
{
	g_engine.state.position.store(0);
}

/* -------------------------------------------------------------------------- */

bool loadAudioFile(std::string path)
{
	auto res = core::makeAudioFile(path, 44100);
	if (!res)
		return false;

	/* Layout first, then data. */
	g_engine.layout.get().audioFile = nullptr;
	g_engine.layout.swap();

	g_engine.data.audioFile         = std::move(res.value());
	g_engine.layout.get().audioFile = &g_engine.data.audioFile;
	g_engine.layout.swap();

	play();

	return true;
}

void unloadAudioFile()
{
	/* Layout first, then data. */
	g_engine.layout.get().audioFile = nullptr;
	g_engine.layout.swap();

	g_engine.data.audioFile = {};

	rewind();
}

/* -------------------------------------------------------------------------- */

void setPitch(float v)
{
	g_engine.layout.get().pitch = std::clamp(v, G_MIN_PITCH, G_MAX_PITCH);
	g_engine.layout.swap();
}

void setPitch(PitchDir dir)
{
	setPitch(g_engine.layout.get().pitch + (dir == PitchDir::UP ? G_PITCH_DELTA : -G_PITCH_DELTA));
}

/* -------------------------------------------------------------------------- */

void nudgePitch_begin(PitchDir dir)
{
	pitchOld_ = g_engine.layout.get().pitch;

	g_engine.layout.get().pitch = pitchOld_ + (dir == PitchDir::UP ? G_PITCH_NUDGE : -G_PITCH_NUDGE);
	g_engine.layout.swap();
}

void nudgePitch_end()
{
	assert(pitchOld_ != 0.0); // Must follow a pitchNudge_begin call

	g_engine.layout.get().pitch = pitchOld_;
	g_engine.layout.swap();

	pitchOld_ = 0.0;
}

/* -------------------------------------------------------------------------- */

void goToFrame(Frame f)
{
	g_engine.state.position.store(f);
}

/* -------------------------------------------------------------------------- */

Frame getCurrentPosition()
{
	return g_engine.state.position.load();
}

Frame getAudioFileLength()
{
	const AudioFile& f = g_engine.data.audioFile;
	return f.isValid() ? f.countFrames() : 0;
}

} // namespace geena::core::api