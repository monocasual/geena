#pragma once


#include <memory>
#include <string>


namespace geena::m
{
class AudioFile
{
public:

    AudioFile();
    AudioFile(std::string path);

private:

    std::unique_ptr<float[]> m_data;
};
} // geena::m::