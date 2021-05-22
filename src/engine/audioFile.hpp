#pragma once


#include <memory>
#include <string>
#include "types.hpp"
#include "deps/audio-buffer/src/audioBuffer.hpp"


using namespace monocasual;


namespace geena::engine
{
class AudioFile
{
public:

    AudioFile();
    AudioFile(AudioBuffer&& data);

    bool isValid() const;
    Frame countFrames() const;
    void render(AudioBuffer& b, Frame start, Frame count, Frame offset=0) const;
    const AudioBuffer& getBuffer() const;

private:

    AudioBuffer m_data;
};
} // geena::engine::