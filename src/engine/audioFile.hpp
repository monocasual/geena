#pragma once


#include <memory>
#include <string>
#include "audioBuffer.hpp"


namespace geena::engine
{
class AudioFile
{
public:

    AudioFile();
    AudioFile(AudioBuffer&& data);

    void render(AudioBuffer& b, Frame start, Frame count, Frame offset=0) const;

private:

    AudioBuffer m_data;
};
} // geena::engine::