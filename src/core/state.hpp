#pragma once

#include "audioFile.hpp"
#include "types.hpp"
#include <atomic>
#include <memory>

namespace geena::core
{
struct State
{
	std::atomic<ReadStatus> status{ReadStatus::STOP};
	std::atomic<Frame>      position{0};
};

struct Layout
{
	float      pitch     = 1.0f;
	AudioFile* audioFile = nullptr;
	State*     state     = nullptr;
};

struct Data
{
	AudioFile audioFile;
};
} // namespace geena::engine