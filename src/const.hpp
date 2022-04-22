#pragma once

namespace geena
{
constexpr float G_UI_REFRESH_RATE = 1 / 24.0f; // 24 times per second

constexpr float G_DEFAULT_PITCH = 1.0;  //   0%
constexpr float G_MIN_PITCH     = 0.84; // -16%
constexpr float G_MAX_PITCH     = 1.16; // +16%

constexpr float G_PITCH_DELTA = 0.0025;
constexpr float G_PITCH_NUDGE = 0.05;
} // namespace geena