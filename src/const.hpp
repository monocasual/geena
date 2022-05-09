#pragma once

#include "types.hpp"

namespace geena
{
constexpr float G_UI_REFRESH_RATE = 1 / 24.0f; // 24 times per second

constexpr float G_DEFAULT_PITCH = 1.0;  //   0%
constexpr float G_MIN_PITCH     = 0.90; // -10%
constexpr float G_MAX_PITCH     = 1.10; // +10%

constexpr float G_PITCH_DELTA = 0.000625;
constexpr float G_PITCH_NUDGE = 0.05;

constexpr Frame G_SEEK_STEP   = 256;
constexpr Frame G_SEEK_LENGTH = 8192;
} // namespace geena