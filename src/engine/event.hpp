#pragma once


#include <variant>


namespace geena::engine
{
enum class EventType 
{
    PLAY, 
    STOP, 
    PAUSE, 
    SET_PITCH,
    SET_AUDIO_FILE,
};


struct Event
{
    EventType type;
    float     fvalue = 0.0f;
    std::shared_ptr<AudioFile> audioFile = nullptr;
};
} // geena::engine::