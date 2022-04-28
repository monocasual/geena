#pragma once

namespace geena
{
enum class PitchDir
{
	UP,
	DOWN
};

enum class ReadStatus
{
	STOP,
	PLAY
};

enum class PlayMode
{
	NORMAL,
	SEEK
};

using Frame = int;

} // namespace geena