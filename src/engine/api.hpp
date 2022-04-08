#pragma once

#include "types.hpp"
#include <string>

namespace geena::engine::api
{
void play();
void stop();
void playPauseToggle();
void rewind();
bool loadAudioFile(std::string path);
void unloadAudioFile();
void setPitch(float v);
void setPitch(PitchDir dir);
void nudgePitch_begin(PitchDir dir);
void nudgePitch_end();
void goToFrame(Frame f);

Frame getCurrentPosition();
Frame getAudioFileLength();
} // namespace geena::engine::api