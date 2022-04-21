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
	g_engine.layout.get().shared->status.store(ReadStatus::PLAY);
}

/* -------------------------------------------------------------------------- */

void pause()
{
	g_engine.layout.get().shared->status.store(ReadStatus::PAUSE);
}

/* -------------------------------------------------------------------------- */

void playPauseToggle()
{
	g_engine.layout.get().shared->status.load() == ReadStatus::PLAY ? pause() : play();
}

/* -------------------------------------------------------------------------- */

void rewind()
{
	g_engine.layout.get().shared->position.store(0);
}

/* -------------------------------------------------------------------------- */

bool loadAudioFile(std::string path)
{
	auto res = core::makeAudioFile(path, 44100);
	if (!res)
		return false;

	/* Disable layout first, then alter data. */
	g_engine.layout.get().enabled = false;
	g_engine.layout.swap();

	g_engine.layout.get().shared->audioFile = std::move(res.value());
	g_engine.layout.get().enabled           = true;
	g_engine.layout.swap();

	return true;
}

void unloadAudioFile()
{
	/* Disable layout first, then alter data. */
	g_engine.layout.get().enabled = false;
	g_engine.layout.swap();

	g_engine.layout.get().shared->audioFile = {};
	g_engine.layout.get().enabled           = true;
	g_engine.layout.swap();

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
	g_engine.layout.get().shared->position.store(f);
}

/* -------------------------------------------------------------------------- */

CurrentState getCurrentState()
{
	const Layout&    layout    = g_engine.layout.get();
	const AudioFile& audioFile = layout.shared->audioFile;

	CurrentState state;
	state.position        = layout.shared->position.load();
	state.audioFileLength = audioFile.isValid() ? audioFile.countFrames() : 0;
	state.pitch           = layout.pitch;

	return state;
}
} // namespace geena::core::api