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
	bool    enabled = true;
	float   pitch   = 1.0f;
	Shared* shared  = nullptr;
};

/* CurrentState
Struct that contains the current state of the model. Used for core -> UI data
passing. */

struct CurrentState
{
	Frame position;
	Frame audioFileLength;
	float pitch;
};
} // namespace geena::core