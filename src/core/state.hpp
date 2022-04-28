#pragma once

#include "audioFile.hpp"
#include "types.hpp"
#include <atomic>
#include <memory>

namespace geena::core
{
struct Shared
{
	std::atomic<ReadStatus> status{ReadStatus::STOP};
	std::atomic<Frame>      position{0};

	AudioFile audioFile;
};

struct Layout
{
	bool     enabled   = true;
	PlayMode playMode  = PlayMode::NORMAL;
	Frame    seekPoint = 0;
	Frame    cuePoint  = 0;
	float    pitch     = 1.0f;
	Shared*  shared    = nullptr;
};

/* CurrentState
Struct that contains the current state of the model. Used for core -> UI data
passing. */

struct CurrentState
{
	ReadStatus  status          = ReadStatus::STOP;
	PlayMode    playMode        = PlayMode::NORMAL;
	Frame       position        = 0;
	Frame       seekPoint       = 0;
	Frame       cuePoint        = 0;
	Frame       audioFileLength = 0;
	std::string audioFilePath;
	float       pitch = 1.0f;
};
} // namespace geena::core