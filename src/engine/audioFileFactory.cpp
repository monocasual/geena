#include <sndfile.h>
#include "audioBuffer.hpp"
#include "audioFileFactory.hpp"


namespace geena::engine
{
std::optional<AudioFile> makeAudioFile(std::string path)
{
    SF_INFO header;
    SNDFILE* sndfile = sf_open(path.c_str(), SFM_READ, &header);

	if (sndfile == nullptr) 
		return {};
	if (header.channels > 2)
		return {};

	AudioBuffer buffer(header.frames, header.channels);

	if (sf_readf_float(sndfile, buffer[0], header.frames) != header.frames)
		printf("Warning: incomplete file read!\n");
	sf_close(sndfile);

    return AudioFile(std::move(buffer));
}
} // geena::engine::