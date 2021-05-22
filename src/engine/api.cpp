#include "api.hpp"
#include "audioFileFactory.hpp"
#include "const.hpp"
#include "src/deps/atomic-swapper/src/atomic-swapper.hpp"
#include "state.hpp"
#include "utils/log.hpp"
#include <cassert>
#include <functional>

using namespace mcl;

namespace geena::engine
{
extern State                 g_state;
extern Data                  g_data;
extern AtomicSwapper<Layout> g_layout;
} // namespace geena::engine

namespace geena::engine::api
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

	/* Layout first, then data. */
	g_layout.get().audioFile = nullptr;
	g_layout.swap();

	g_data.audioFile         = std::move(res.value());
	g_layout.get().audioFile = &g_data.audioFile;
	g_layout.swap();

	play();

	return true;
}

void unloadAudioFile()
{
	/* Layout first, then data. */
	g_layout.get().audioFile = nullptr;
	g_layout.swap();

	g_data.audioFile = {};

	rewind();
}

/* -------------------------------------------------------------------------- */

void setPitch(PitchDir dir)
{
	g_layout.get().pitch += dir == PitchDir::UP ? G_PITCH_DELTA : -G_PITCH_DELTA;
	g_layout.swap();
}

void nudgePitch_begin(PitchDir dir)
{
	pitchOld_ = g_layout.get().pitch;

	g_layout.get().pitch = pitchOld_ + (dir == PitchDir::UP ? G_PITCH_NUDGE : -G_PITCH_NUDGE);
	g_layout.swap();
}

void nudgePitch_end()
{
	assert(pitchOld_ != 0.0); // Must follow a pitchNudge_begin call

	g_layout.get().pitch = pitchOld_;
	g_layout.swap();

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

} // namespace geena::engine::api