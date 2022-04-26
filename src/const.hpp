#pragma once

namespace geena
{
constexpr float G_UI_REFRESH_RATE = 1 / 24.0f; // 24 times per second

constexpr float G_DEFAULT_PITCH = 1.0;  //   0%
constexpr float G_MIN_PITCH     = 0.90; // -10%
constexpr float G_MAX_PITCH     = 1.10; // +10%

constexpr float G_PITCH_DELTA = 0.000625;
constexpr float G_PITCH_NUDGE = 0.05;
} // namespace geena