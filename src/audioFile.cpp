#include <sndfile.h>
#include "audioFile.hpp"


namespace geena::m
{
AudioFile::AudioFile()
{

}


/* -------------------------------------------------------------------------- */


AudioFile::AudioFile(std::string path)
{
	SF_INFO  header;
	SNDFILE* file = sf_open(path.c_str(), SFM_READ, &header);

    m_data = std::make_unique<float[]>(header.frames * header.channels);

	if (sf_readf_float(file, m_data.get(), header.frames) != header.frames)
		printf("Warning: incomplete read!\n");

	sf_close(file);
}
} // geena::m::