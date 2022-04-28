#include "core/api.hpp"
#include "const.hpp"
#include "core/audioFileFactory.hpp"
#include "core/engine.hpp"
#include "core/state.hpp"
#include "deps/atomic-swapper/src/atomic-swapper.hpp"
#include "deps/mcl-utils/src/log.hpp"
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

/* -------------------------------------------------------------------------- */

void setCueAtFrame_(Frame f)
{
	Layout& layout = g_engine.layout.get();

	layout.cuePoint  = f;
	layout.playMode  = PlayMode::NORMAL;
	layout.seekPoint = 0;
	layout.shared->position.store(layout.cuePoint);

	g_engine.layout.swap();
}
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
	g_engine.layout.get().shared->status.store(ReadStatus::STOP);
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
	AudioFile audioFile = res.value();

	/* Disable layout first, then alter data. */
	g_engine.layout.get().enabled = false;
	g_engine.layout.swap();

	g_engine.renderer.init(core::g_engine.kernel.getConfig().bufferSize, audioFile.countChannels());

	g_engine.layout.get().shared->audioFile = std::move(audioFile);
	g_engine.layout.get().shared->status.store(ReadStatus::STOP);
	g_engine.layout.get().shared->position.store(0);
	g_engine.layout.get().enabled = true;
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

float setPitch(PitchDir dir)
{
	float v = g_engine.layout.get().pitch + (dir == PitchDir::UP ? G_PITCH_DELTA : -G_PITCH_DELTA);
	setPitch(v);
	return v;
}

/* -------------------------------------------------------------------------- */

void nudgePitch_begin(PitchDir dir)
{
	Layout&          layout     = g_engine.layout.get();
	const ReadStatus readStatus = layout.shared->status.load();

	/* If paused, start SEEK mode. */

	if (readStatus == ReadStatus::STOP)
	{
		layout.playMode  = PlayMode::SEEK;
		layout.seekPoint = layout.shared->position.load();
		layout.shared->status.store(ReadStatus::PLAY);
	}
	else
	{
		if (layout.playMode == PlayMode::SEEK)
		{
			layout.seekPoint = std::max(0, layout.seekPoint + (dir == PitchDir::UP ? G_SEEK_STEP : -G_SEEK_STEP));
		}
		else
		{
			pitchOld_    = layout.pitch;
			layout.pitch = pitchOld_ + (dir == PitchDir::UP ? G_PITCH_NUDGE : -G_PITCH_NUDGE);
		}
	}

	g_engine.layout.swap();
}

void nudgePitch_end()
{
	/* Must follow a pitchNudge_begin call. If not, it means the previous 
	pitchNudge_begin was used to start SEEK mode: skip this. */

	if (pitchOld_ == 0.0)
		return;

	g_engine.layout.get().pitch = pitchOld_;
	g_engine.layout.swap();

	pitchOld_ = 0.0;
}

/* -------------------------------------------------------------------------- */

void goToFrame(Frame f)
{
	ML_DEBUG("Go to frame " << f);

	g_engine.layout.get().shared->position.store(f);
}

/* -------------------------------------------------------------------------- */

void setCue()
{
	const Layout&    layout     = g_engine.layout.get();
	const ReadStatus readStatus = layout.shared->status.load();
	const Frame      position   = layout.shared->position.load();

	if (readStatus == ReadStatus::STOP)
	{
		ML_DEBUG("Set cue at current position " << position);

		setCueAtFrame_(position);
	}
	else // ReadStatus::PLAY
	{
		if (layout.playMode == PlayMode::NORMAL)
		{
			ML_DEBUG("Jump to cue at frame " << layout.cuePoint);

			layout.shared->position.store(layout.cuePoint);
		}
		else // PlayMode::SEEK
		{
			ML_DEBUG("Set cue at seek point " << layout.seekPoint << " (SEEK mode)");

			setCueAtFrame_(layout.seekPoint);
		}

		layout.shared->status.store(ReadStatus::STOP);
	}
}

/* -------------------------------------------------------------------------- */

CurrentState getCurrentState()
{
	const Layout&    layout    = g_engine.layout.get();
	const AudioFile& audioFile = layout.shared->audioFile;

	CurrentState state;
	state.status          = layout.shared->status.load();
	state.playMode        = layout.playMode;
	state.position        = layout.shared->position.load();
	state.seekPoint       = layout.seekPoint;
	state.cuePoint        = layout.cuePoint;
	state.audioFileLength = audioFile.isValid() ? audioFile.countFrames() : 0;
	state.audioFilePath   = audioFile.isValid() ? audioFile.getPath() : "";
	state.pitch           = layout.pitch;

	return state;
}
} // namespace geena::core::api