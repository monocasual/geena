#include "audioFileFactory.hpp"
#include "deps/mcl-utils/src/log.hpp"
#include <cmath>
#include <samplerate.h>
#include <sndfile.h>

using namespace mcl;

namespace geena::engine
{
namespace
{
bool resample(AudioBuffer& b, int oldSampleRate, int newSampleRate)
{
	float ratio   = newSampleRate / (float)oldSampleRate;
	Frame newSize = static_cast<int>(std::ceil(b.countFrames() * ratio));

	AudioBuffer newData(newSize, b.countChannels());

	SRC_DATA src_data;
	src_data.data_in       = b[0];
	src_data.input_frames  = b.countFrames();
	src_data.data_out      = newData[0];
	src_data.output_frames = newSize;
	src_data.src_ratio     = ratio;

	ML_DEBUG("Resampling: new size=" << newSize << " frames");

	int ret = src_simple(&src_data, SRC_SINC_FASTEST, b.countChannels());
	if (ret != 0)
	{
		ML_DEBUG("Resampling error: " << src_strerror(ret));
		return false;
	}

	b = std::move(newData);

	return true;
}
} // namespace

/* -------------------------------------------------------------------------- */
/* -------------------------------------------------------------------------- */
/* -------------------------------------------------------------------------- */

std::optional<AudioFile> makeAudioFile(std::string path, int sampleRate)
{
	SF_INFO  header;
	SNDFILE* sndfile = sf_open(path.c_str(), SFM_READ, &header);

	if (sndfile == nullptr)
	{
		ML_DEBUG("Can't open file " << path);
		return {};
	}
	if (header.channels > 2)
	{
		ML_DEBUG("Unsupported number of audio channels");
		return {};
	}

	AudioBuffer buffer(header.frames, header.channels);

	if (sf_readf_float(sndfile, buffer[0], header.frames) != header.frames)
		ML_DEBUG("Warning: incomplete file read!\n");
	sf_close(sndfile);

	if (header.samplerate != sampleRate)
		if (!resample(buffer, header.samplerate, sampleRate))
		{
			ML_DEBUG("Error while resampling audio");
			return {};
		}

	ML_DEBUG("AudioFile ready");
	return {AudioFile(std::move(buffer))};
}
} // namespace geena::engine