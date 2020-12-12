#include <cmath>
#include <sndfile.h>
#include <samplerate.h>
#include "audioBuffer.hpp"
#include "audioFileFactory.hpp"


namespace geena::engine
{
namespace
{
bool resample(AudioBuffer& b, int oldSampleRate, int newSampleRate)
{
	float ratio = newSampleRate / (float) oldSampleRate;
	Frame newSize = static_cast<int>(std::ceil(b.countFrames() * ratio));

	AudioBuffer newData(newSize, b.countChannels());

	SRC_DATA src_data;
	src_data.data_in       = b[0];
	src_data.input_frames  = b.countFrames();
	src_data.data_out      = newData[0];
	src_data.output_frames = newSize;
	src_data.src_ratio     = ratio;

	//u::log::print("[waveManager::resample] resampling: new size=%d frames\n", newSizeFrames);

	int ret = src_simple(&src_data, SRC_SINC_FASTEST, b.countChannels());
	if (ret != 0) {
		//u::log::print("[waveManager::resample] resampling error: %s\n", src_strerror(ret));
		return false;
	}

	b = std::move(newData);

	return true;
}
} // {anonymous}


/* -------------------------------------------------------------------------- */
/* -------------------------------------------------------------------------- */
/* -------------------------------------------------------------------------- */


std::optional<AudioFile> makeAudioFile(std::string path, int sampleRate)
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

	if (header.samplerate != sampleRate)
		if (!resample(buffer, header.samplerate, sampleRate))
			return {};

    return AudioFile(std::move(buffer));
}
} // geena::engine::