#pragma once

#include "core/state.hpp"
#include "types.hpp"
#include <string>

namespace geena::core::api
{
void  play();
void  pause();
void  playPauseToggle();
void  rewind();
bool  loadAudioFile(std::string path);
void  unloadAudioFile();
void  setPitch(float v);
float setPitch(PitchDir dir);
void  nudgePitch_begin(PitchDir dir);
void  nudgePitch_end();
void  goToFrame(Frame f);

/* setCue
Sets cue point to the current position. Also turns off SEEK mode (if currently 
enabled). */

void setCue(bool isKeyPressed);

CurrentState getCurrentState();
} // namespace geena::core::api