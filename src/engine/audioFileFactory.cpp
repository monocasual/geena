#include <cmath>
#include <sndfile.h>
#include <samplerate.h>
#include "utils/log.hpp"
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

	G_DEBUG("Resampling: new size=" << newSize << " frames");

	int ret = src_simple(&src_data, SRC_SINC_FASTEST, b.countChannels());
	if (ret != 0) {
		G_DEBUG("Resampling error: " << src_strerror(ret));
		return false;
	}

	b = std::move(newData);

	return true;
}
} // {anonymous}


/* -------------------------------------------------------------------------- */
/* -------------------------------------------------------------------------- */
/* -------------------------------------------------------------------------- */


std::shared_ptr<AudioFile> makeAudioFile(std::string path, int sampleRate)
{
    SF_INFO header;
    SNDFILE* sndfile = sf_open(path.c_str(), SFM_READ, &header);

	if (sndfile == nullptr) 
	{
		G_DEBUG("Can't open file " << path);
		return {};
	}
	if (header.channels > 2)
	{
		G_DEBUG("Unsupported number of audio channels");
		return {};
	}

	AudioBuffer buffer(header.frames, header.channels);

	if (sf_readf_float(sndfile, buffer[0], header.frames) != header.frames)
		G_DEBUG("Warning: incomplete file read!\n");
	sf_close(sndfile);

	if (header.samplerate != sampleRate)
		if (!resample(buffer, header.samplerate, sampleRate))
		{
			G_DEBUG("Error while resampling audio");
			return {};
		}

	G_DEBUG("AudioFile ready");
    return std::make_shared<AudioFile>(std::move(buffer));
}
} // geena::engine::